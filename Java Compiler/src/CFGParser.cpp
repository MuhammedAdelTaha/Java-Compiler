// CFGParser.cpp
#include "CFGParser.h"
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <queue>
#include <set>

CFGParser::CFGParser(const Grammar& grammar) : m_Grammar(grammar) {}

void CFGParser::eliminateLeftRecursion() {
    std::vector<std::string> nonTerminals = getNonTerminalOrder();

    // Iterate over each non-terminal
    for (size_t i = 0; i < nonTerminals.size(); ++i) {
        const std::string& Ai = nonTerminals[i];

        // Replace productions starting with Aj where j < i
        for (size_t j = 0; j < i; ++j) {
            const std::string& Aj = nonTerminals[j];
            std::vector<Production> newProductions;

            for (const auto& production : m_Grammar[Ai]) {
                if (production.empty()) {
                    newProductions.push_back(production);
                    continue;
                }

                //if (production[0].type == Symbol::Type::NonTerminal && production[0].name == Aj) {
                //    // Replace Aj with its productions
                //    for (const auto& AjProduction : m_Grammar[Aj]) {
                //        Production newProd = AjProduction;
                //        newProd.insert(newProd.end(), production.begin() + 1, production.end());
                //        newProductions.push_back(newProd);
                //    }
                //} else {
                    newProductions.push_back(production);
                //}
            }

            // Update productions after replacement
            m_Grammar[Ai] = newProductions;
        }

        // Now eliminate immediate left recursion for Ai
        // Separate productions into those that start with Ai and those that don't
        std::vector<Production> recursiveProds;
        std::vector<Production> nonRecursiveProds;

        for (const auto& production : m_Grammar[Ai]) {
            if (!production.empty() && production[0].type == Symbol::Type::NonTerminal && production[0].name == Ai) {
                // Production is recursive
                recursiveProds.emplace_back(production.begin() + 1, production.end());
            } else {
                nonRecursiveProds.emplace_back(production);
            }
        }

        // If there are recursive productions, eliminate immediate left recursion
        if (!recursiveProds.empty()) {
            // Create a new non-terminal Ai'
            std::string AiPrime = generateNewNonTerminal(Ai);

            // Update non-recursive productions to include Ai'
            std::vector<Production> updatedNonRecursiveProds;
            for (auto& prod : nonRecursiveProds) {
                Production newProd = prod;
                // Append Ai' to the end
                Symbol AiPrimeSymbol;
                AiPrimeSymbol.type = Symbol::Type::NonTerminal;
                AiPrimeSymbol.name = AiPrime;
                newProd.push_back(AiPrimeSymbol);
                updatedNonRecursiveProds.push_back(newProd);
            }

            // Create new productions for Ai'
            std::vector<Production> AiPrimeProds;
            for (auto& prod : recursiveProds) {
                // Append Ai' to each recursive production
                prod.push_back(Symbol{ Symbol::Type::NonTerminal, AiPrime });
                AiPrimeProds.push_back(prod);
            }
            // Add epsilon (empty production) to Ai'
            AiPrimeProds.emplace_back(); // Represents epsilon

            // Update the grammar
            m_Grammar[Ai] = updatedNonRecursiveProds;
            m_Grammar[AiPrime] = AiPrimeProds;
        }
    }
}

void CFGParser::leftFactor() {
    // Initialize a queue with all existing non-terminals
    std::queue<std::string> toProcess;
    std::set<std::string> processed;

    for (const auto& pair : m_Grammar) {
        toProcess.push(pair.first);
    }

    while (!toProcess.empty()) {
        std::string nonTerminal = toProcess.front();
        toProcess.pop();

        // Avoid processing the same non-terminal multiple times
        if (processed.find(nonTerminal) != processed.end()) {
            continue;
        }

        processed.insert(nonTerminal);

        // Perform left factoring on the non-terminal
        std::vector<std::string> newNonTerminals = leftFactorNonTerminal(nonTerminal);

        // Enqueue the newly created nonTerminals for further factoring
        for (const auto& nt : newNonTerminals) {
            if (processed.find(nt) == processed.end()) {
                toProcess.push(nt);
            }
        }
    }
}

CFGParser::Grammar CFGParser::getGrammar() const {
    return m_Grammar;
}

std::string CFGParser::generateNewNonTerminal(const std::string& nonTerminal) {
    // Initialize the new non-terminal name with one apostrophe
    std::string newNonTerminal = nonTerminal + "'";

    // Append additional apostrophes if the new non-terminal already exists
    while (m_Grammar.find(newNonTerminal) != m_Grammar.end()) {
        newNonTerminal += "'";
    }

    return newNonTerminal;
}

bool CFGParser::isNonTerminal(const std::string& symbol) const {
    // Assuming that non-terminals are keys in the grammar map
    return m_Grammar.find(symbol) != m_Grammar.end();
}

std::vector<std::string> CFGParser::getNonTerminalOrder() const {
    std::vector<std::string> order;
    for (const auto& pair : m_Grammar) {
        order.emplace_back(pair.first);
    }
    return order;
}

std::vector<std::string> CFGParser::leftFactorNonTerminal(const std::string& nonTerminal) {
    std::vector<std::string> createdNonTerminals;
    bool factored;

    do {
        factored = false;
        const auto& productions = m_Grammar[nonTerminal];

        // Map from prefix to productions that have that prefix
        std::unordered_map<std::string, std::vector<Production>> prefixMap;

        // Build the prefix map (consider only the first symbol)
        for (const auto& prod : productions) {
            if (prod.empty()) continue;
            std::string prefix = prod[0].name;
            prefixMap[prefix].push_back(prod);
        }

        // Identify the prefix with the maximum number of productions
        std::string commonPrefix;
        size_t maxCount = 1; // Only consider prefixes appearing more than once

        for (const auto& pair : prefixMap) {
            if (pair.second.size() > maxCount) {
                commonPrefix = pair.first;
                maxCount = pair.second.size();
            }
        }

        // If a common prefix is found (appears in more than one production)
        if (!commonPrefix.empty()) {
            factored = true;

            // Create a new non-terminal for the factored productions
            std::string newNonTerminal = generateNewNonTerminal(nonTerminal);
            createdNonTerminals.push_back(newNonTerminal);

            std::vector<Production> newProductions;      // Productions without the common prefix
            std::vector<Production> factoredProductions; // Productions to be factored

            for (const auto& prod : productions) {
                if (!prod.empty() && prod[0].name == commonPrefix) {
                    // Remove the common prefix and add to factored productions
                    Production newProd(prod.begin() + 1, prod.end());
                    factoredProductions.push_back(newProd);
                } else {
                    // Keep the production as is
                    newProductions.push_back(prod);
                }
            }

            // Update the original non-terminal's productions
            std::vector<Production> updatedProductions;
            // Add the factored out common prefix with the new non-terminal
            Production prefixProd;
            Symbol prefixSymbol;

            // Determine if the common prefix is a terminal or non-terminal
            if (isNonTerminal(commonPrefix)) {
                prefixSymbol.type = Symbol::Type::NonTerminal;
            } else {
                prefixSymbol.type = Symbol::Type::Terminal;
            }
            prefixSymbol.name = commonPrefix;
            prefixProd.push_back(prefixSymbol);

            Symbol newNonTerminalSymbol;
            newNonTerminalSymbol.type = Symbol::Type::NonTerminal;
            newNonTerminalSymbol.name = newNonTerminal;
            prefixProd.push_back(newNonTerminalSymbol);

            updatedProductions.push_back(prefixProd);
            // Add the non-factored productions
            updatedProductions.insert(updatedProductions.end(), newProductions.begin(), newProductions.end());

            // Update the grammar with the new productions
            m_Grammar[nonTerminal] = updatedProductions;

            // Add the new non-terminal with its productions
            m_Grammar[newNonTerminal] = factoredProductions;
        }

    } while (factored); // Repeat until no more factoring can be done

    return createdNonTerminals;
}
