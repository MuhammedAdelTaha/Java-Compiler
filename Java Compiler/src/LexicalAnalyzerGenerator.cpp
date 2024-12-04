#include <fstream>
#include <sstream>
#include <stack>
#include <cctype>
#include "Utilities.h"
#include "RegexHandler.h"
#include "LexicalAnalyzerGenerator.h"

LexicalAnalyzerGenerator::LexicalAnalyzerGenerator(const std::string& filePath)
    : m_FilePath(filePath)
{
	readRulesFromFile();
	generateDFA();
}

// Reads the rules from the file and writes them to the corresponding data structures.
void LexicalAnalyzerGenerator::readRulesFromFile() 
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

    while (std::getline(file, line)) 
    {
        // Remove leading and trailing whitespaces.
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) 
            continue;

        // Process different types of rules.
        if (line.find(':') != std::string::npos)
            processRegularExpression(line);
        else if (line.find('=') != std::string::npos)
            processRegularDefinition(line);
        else if (line[0] == '{' && line.back() == '}')
            processKeywords(line);
        else if (line[0] == '[' && line.back() == ']')
            processPunctuations(line);
    }
}

// Processes a regular definition and adds it to the regular definitions map.
void LexicalAnalyzerGenerator::processRegularDefinition(const std::string& line) 
{
    // Trim all whitespaces.
	std::string trimmedLine = line;
	trimmedLine.erase(std::remove_if(trimmedLine.begin(), trimmedLine.end(), ::isspace), trimmedLine.end());

    size_t pos = trimmedLine.find('=');
    std::string lhs = trimmedLine.substr(0, pos);
    std::string rhs = trimmedLine.substr(pos + 1);

	m_RegularDefinitions[lhs] = rhs;
}

// Processes a regular expression and adds it to the regular expressions map.
void LexicalAnalyzerGenerator::processRegularExpression(const std::string& line) 
{
    // Trim all whitespaces.
    std::string trimmedLine = line;
    trimmedLine.erase(std::remove_if(trimmedLine.begin(), trimmedLine.end(), ::isspace), trimmedLine.end());

    size_t pos = trimmedLine.find(':');
    std::string lhs = trimmedLine.substr(0, pos);
    std::string rhs = trimmedLine.substr(pos + 1);

    // Push the token name in the tokens precedence vector, which determines the order, where the tokens were defined.
	m_TokensPrecedence[lhs] = m_TokensPrecedence.size() + 1;

    m_RegularExpressions[lhs] = rhs;
}

// Processes a list of keywords and adds them to the keywords vector.
void LexicalAnalyzerGenerator::processKeywords(const std::string& line) 
{
    std::istringstream ss(line.substr(1, line.length() - 2));
    std::string keyword;

    while (ss >> keyword)
        m_Keywords.insert(keyword);
}

// Processes a list of punctuations and adds them to the punctuations vector.
void LexicalAnalyzerGenerator::processPunctuations(const std::string& line) 
{
    std::istringstream ss(line.substr(1, line.length() - 2));
    std::string punctuation;

    while (ss >> punctuation)
    {
		if (punctuation[0] == '\\')
			m_Punctuations.insert(punctuation.substr(1, punctuation.size() - 1));
        else
            m_Punctuations.insert(punctuation);
    }
}

// Generates DFAs for all regular expressions.
void LexicalAnalyzerGenerator::generateDFA()
{
    std::vector<NFA> nfas;
    std::map<std::string, std::set<std::shared_ptr<State>>> regexTerminalStates;

    // Generate DFA for regular expressions.
    for (auto [name, regex] : m_RegularExpressions)
    {
        std::string postfixExp = RegexHandler::infixToPostfix(regex);
        NFA nfa = convertRegexToNFA(postfixExp);
		nfas.push_back(nfa);
		regexTerminalStates[name] = nfa.getTerminalStates();
    }
	NFA combinedNFA = combineNFAs(nfas);
	m_DFA = DFA(combinedNFA);
	
	// Map every regex to its terminal closures in the DFA.
	for (auto [name, terminalStates] : regexTerminalStates)
		for (auto terminalState : terminalStates)
            for (auto terminalClosure : m_DFA.getTerminalClosures())
                if (terminalClosure.count(terminalState))
				    m_RegexEpsilonClosures[name].insert(terminalClosure);
}

// Combines all NFAs into a single NFA.
NFA LexicalAnalyzerGenerator::combineNFAs(const std::vector<NFA>& nfas)
{
	std::shared_ptr<State> startState = std::make_shared<State>();
	std::set<std::shared_ptr<State>> terminalStates;

    for (auto nfa : nfas)
    {
        startState->addTransition(nfa.getStartState(), 0);
		std::set<std::shared_ptr<State>> nfaTerminalStates = nfa.getTerminalStates();
		terminalStates.insert(nfaTerminalStates.begin(), nfaTerminalStates.end());
    }
    return NFA(startState, terminalStates);
}

// Converts a regular definition to an NFA.
NFA LexicalAnalyzerGenerator::convertDefToNFA(const std::string& def) 
{
	std::shared_ptr<State> startState = std::make_shared<State>();
	std::shared_ptr<State> terminalState = std::make_shared<State>();
    for (size_t i = 0; i < def.length(); i++) 
    {
        if (def[i] == '|')
            continue;
        if (def[i] == '-') 
        {
            char startSymbol = def[i - 1];
            char endSymbol = def[i + 1];
            for (char c = startSymbol; c <= endSymbol; c++)
				startState->addTransition(terminalState, c);
        }
    }
    return NFA(startState, { terminalState });
}

// Converts a symbol to an NFA.
NFA LexicalAnalyzerGenerator::convertSymbolToNFA(const std::string& word)
{
    std::string sym = word;
	if (word[0] == '\\')
    {
        if (word[1] == 'L') // Epsilon (Lambda) transition.
			sym = std::string(1, 0);
        else
            sym = word.substr(1, word.size() - 1);
	}

    std::shared_ptr<State> startState = std::make_shared<State>();

    std::shared_ptr<State> currentState = startState;
    std::shared_ptr<State> nextState = std::make_shared<State>();
	for (size_t i = 0; i < sym.length(); i++)
	{
		char c = sym[i];
		currentState->addTransition(nextState, c);
		
        if (i == sym.length() - 1)
			break;

		currentState = nextState;
		nextState = std::make_shared<State>();
	}

	std::shared_ptr<State> terminalState = nextState;

    return NFA(startState, { terminalState });
}

// Converts a regular expression to an NFA.
NFA LexicalAnalyzerGenerator::convertRegexToNFA(const std::string& postfixExp) 
{
    // Split the expression on whitespaces.
    std::vector<std::string> words = splitWords(postfixExp);

    // Stack to handle postfix expressions conversion.
    std::stack<NFA> nfaStack;

    for (std::string word : words)
    {
        if (m_RegularDefinitions.count(word)) // Regular definitions.
        {
            nfaStack.push(convertDefToNFA(m_RegularDefinitions[word]));
        }
        else if (word == "*" && !nfaStack.empty()) // Kleene closure.
        {
            NFA top = nfaStack.top();
            nfaStack.pop();
            top.kleeneClosure();
            nfaStack.push(top);
        }
        else if (word == "+" && !nfaStack.empty()) // Positive closure.
        {
            NFA top = nfaStack.top();
            nfaStack.pop();
            top.positiveClosure();
            nfaStack.push(top);
        }
        else if (word == "." && nfaStack.size() >= 2) // Concatenation.
        {
            NFA right = nfaStack.top();
            nfaStack.pop();
            NFA left = nfaStack.top();
            nfaStack.pop();
            left.concatenate(right);
            nfaStack.push(left);
        }
        else if (word == "|" && nfaStack.size() >= 2) // Union.
        {
            NFA right = nfaStack.top();
            nfaStack.pop();
            NFA left = nfaStack.top();
            nfaStack.pop();
            left.unionize(right);
            nfaStack.push(left);
        }
        else // Other terminal symbols.
        {
            nfaStack.push(convertSymbolToNFA(word));
        }
    }

    // Final NFA will be on top of the stack
    return nfaStack.top();
}
