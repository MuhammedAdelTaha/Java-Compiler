#include "ParseTableGenerator.h"
#include "CFGParser.h"
#include <iostream>

ParseTableGenerator::ParseTableGenerator(const std::string& filePath) 
	: m_FilePath(filePath)
{
	readRulesFromFile();
	CFGParser cfgParser(m_Grammar);
	cfgParser.eliminateLeftRecursion();
	cfgParser.leftFactor();
	m_Grammar = cfgParser.getGrammar();
}

void ParseTableGenerator::readRulesFromFile()
{
	std::ifstream file(m_FilePath);
	std::string line;

	if (file.peek() == 0xEF)
	{
		char bom[3];
		file.read(bom, 3);
		if (!(bom[0] == (char)0xEF && bom[1] == (char)0xBB && bom[2] == (char)0xBF))
			file.seekg(0);
	}

	bool isStartSymbol = true;
	while (std::getline(file, line))
	{
		// Remove leading and trailing whitespaces.
		line = trimStringEdges(line);

		if (line.empty())
			continue;

		// Process different types of rules.
		size_t pos = line.find('=');
		std::string lhs = line.substr(0, pos);
		std::string rhs = line.substr(pos + 1);
		if (isStartSymbol)
		{
			m_StartSymbol = { .type = Symbol::Type::NonTerminal, .name = trimStringEdges(lhs) };
			isStartSymbol = false;
		}
		m_Grammar[trimStringEdges(lhs)] = getProductions(rhs);
	}
}

std::vector<std::vector<Symbol>> ParseTableGenerator::getProductions(const std::string& str)
{
	std::vector<std::vector<Symbol>> productions;
	std::vector<std::string> productionsStrings = split(str, '|');

	for (auto productionStr : productionsStrings)
	{
		std::vector<std::string> symbols = split(productionStr, ' ');
		std::vector<Symbol> production;
		for (auto symbolName : symbols)
		{
			if (symbolName[0] == '\'')
				production.push_back({ .type = Symbol::Type::Terminal, .name = symbolName.substr(1, symbolName.size() - 2) });
			else
				production.push_back({ .type = Symbol::Type::NonTerminal, .name = symbolName });
		}
		productions.push_back(production);
	}

	return productions;
}

void ParseTableGenerator::generateFirstTable()
{
	for (auto& [nonTerminal, productions] : m_Grammar)
	{
		generateFirstSet(nonTerminal, productions);
	}
}

void ParseTableGenerator::generateFollowTable(Symbol startSymbol)
{	
	std::unordered_map<std::string, std::vector<std::string>> dependecnies;
	for (auto& [nonTerminal, _] : m_Grammar)
	{
		generateFollowSet(dependecnies,nonTerminal, startSymbol);
	}
	checkConvergance(dependecnies);
}

void ParseTableGenerator::generateParseTable()
{
	for (auto& [nonTerminal, productions] : m_Grammar)
	{
		bool hasEps = false;
		for (std::vector<Symbol>& production : productions)
		{
			if (production.size() == 0)
				production = { Symbol{ .type=Symbol::Type::Terminal, .name="\\L" } };

			if (production[0].name == "\\L")
			{	
				std::unordered_set<std::string> productionFollowSet = m_FollowTable[nonTerminal];
				for (std::string follow : productionFollowSet)
				{
					if (m_ParseTable[nonTerminal][follow].size() == 0)
					{
						m_ParseTable[nonTerminal][follow] = production;
					}
					else
					{
						std::cerr << "ERROR: Grammar is not LL(1)" << std::endl;
					}
				}
				hasEps = true;
				continue;
			}

			std::unordered_set<std::string> productionFirstSet = m_ProductionFirstTable[production];
			for (std::string first : productionFirstSet)
			{
				if (m_ParseTable[nonTerminal][first].size() == 0)
				{
					m_ParseTable[nonTerminal][first] = production;
				}
				else
				{
					std::cerr << "ERROR: Grammar is not LL(1)" << std::endl;
				}
			}
		}

		if (hasEps) continue;

		std::unordered_set<std::string> productionFollowSet = m_FollowTable[nonTerminal];
		for (std::string follow : productionFollowSet)
		{
			if (m_ParseTable[nonTerminal][follow].size() == 0)
			{
				m_ParseTable[nonTerminal][follow] = { {.type=Symbol::Type::Terminal,.name="\\S"} };
			}
		}
	}
}

void ParseTableGenerator::generateFirstSet(std::string nonTerminal, std::vector<std::vector<Symbol>>& productions)
{
	for (std::vector<Symbol>& production : productions)
	{
		if (production.size() == 0)
			production = { Symbol{.type = Symbol::Type::Terminal, .name = "\\L" } };

		addFirst(nonTerminal, production);
	}
}

void ParseTableGenerator::addFirst(std::string nonTerminal, std::vector<Symbol>& production)
{

	if (production[0].type == Symbol::Type::Terminal)
	{
		m_FirstTable[nonTerminal].insert(production[0].name);
		m_ProductionFirstTable[production].insert(production[0].name);
	}
	else
	{
		for (Symbol symbol : production)
		{
			if (m_FirstTable[symbol.name].size() == 0)
			{
				generateFirstSet(symbol.name, m_Grammar[symbol.name]);
			}
			m_FirstTable[nonTerminal].insert(m_FirstTable[symbol.name].begin(), m_FirstTable[symbol.name].end());
			m_ProductionFirstTable[production].insert(m_FirstTable[symbol.name].begin(), m_FirstTable[symbol.name].end());
			if (!m_FirstTable[symbol.name].contains("\\L"))
				break;
		}

	}
}


void ParseTableGenerator::generateFollowSet(std::unordered_map<std::string, std::vector<std::string>> &dependecnies
									,std::string nonTerminal, Symbol startSymbol)
{
	for (auto& [otherNonTerminal, productions] : m_Grammar)
	{
		for (std::vector<Symbol> production : productions)
		{
			bool isNonTerminalFound = false;
			for (int i = 0; i < production.size() + 1; i++)
			{
				if (i < production.size() && !isNonTerminalFound && production[i].name == nonTerminal)
				{
					isNonTerminalFound = true;
				}
				else if (isNonTerminalFound)
				{
					// If non terminal found at the last position of the production
					if (i == production.size())
					{
						dependecnies[otherNonTerminal].push_back(nonTerminal);
						break;
					}
					// If non terminal found at any inner positions of the production and the next symbol is a terminal
					else if (production[i].type == Symbol::Type::Terminal)
					{
						m_FollowTable[nonTerminal].insert(production[i].name);
						break;
					}
					// If non terminal found at any inner positions of the production and the next symbol is a non-Terminal
					else if (production[i].type == Symbol::Type::NonTerminal)
					{
						m_FollowTable[nonTerminal].insert(m_FirstTable[production[i].name].begin(), m_FirstTable[production[i].name].end());
						if (!m_FirstTable[production[i].name].contains("\\L"))
						{
							break;
						}
					}
				}
			}
		}
	}

	if (nonTerminal == startSymbol.name)
	{
		m_FollowTable[nonTerminal].insert("$");
	}

	m_FollowTable[nonTerminal].erase("\\L");
}

void ParseTableGenerator::checkConvergance(std::unordered_map<std::string, std::vector<std::string>>& dependecnies) {
	bool converged = false;

	while (!converged) {
		converged = true;

		// Temporary storage for the new values
		std::unordered_map<std::string, std::unordered_set<std::string>> newValues = m_FollowTable;

		for (const auto& [node, neighbors] : dependecnies) {
			for (const  std::string& neighbor : neighbors) {
				// Propagate values and check if any changes occurred
				if (propagateValues(m_FollowTable[node], newValues[neighbor])) {
					converged = false;
				}
			}
		}

		// Update node values for the next iteration
		m_FollowTable = std::move(newValues);
	}
}

bool ParseTableGenerator::propagateValues(const std::unordered_set<std::string>& src, std::unordered_set<std::string>& dst) {
	bool changed = false;
	for (std::string value : src) {
		// Insert value if not already in dst
		if (dst.insert(value).second) {
			changed = true; // If a new value was added, mark as changed
		}
	}
	return changed;
}

void ParseTableGenerator::printFirstTable()
{
	for (auto [nonTerminal, firstSet] : m_FirstTable)
	{
		std::cout << nonTerminal << ":" << std::endl;
		for (std::string first : firstSet)
		{
			std::cout << first << " ";
		}
		std::cout << std::endl;
		std::cout << "===========================================" << std::endl;
	}
}

void ParseTableGenerator::printFollowTable()
{
	for (auto [nonTerminal, followSet] : m_FollowTable)
	{
		std::cout << nonTerminal << ":" << std::endl;
		for (std::string follow : followSet)
		{
			std::cout << follow << " ";
		}
		std::cout << std::endl;
		std::cout << "===========================================" << std::endl;
	}
}

void ParseTableGenerator::printParseTable()
{
	std::cout << "Parse Table:\n";
	for (auto& [nonTerminal, Transitions] : m_ParseTable)
	{
		if (!nonTerminal.starts_with("TERM"))
			continue;
		std::cout << nonTerminal << ":" << std::endl;
		for (auto& [terminal, production] : Transitions)
		{
			std::cout << "\t" << terminal << ":" << std::endl;
			std::cout << "\t\t";
			for (Symbol symbol : production) 
			{
				std::cout << symbol.name << "  ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << "===========================================" << std::endl;
	}
}
