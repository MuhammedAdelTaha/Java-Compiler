#include "LexicalAnalyzer.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>

LexicalAnalyzer::LexicalAnalyzer(std::string& inputText, std::map<std::string, DFA>& tokenDFAs)
    : input(inputText), tokenDFAs(tokenDFAs), currentPosition(0)
{
}

//// Main method to get the next token from the input
//std::pair<std::string, std::string> LexicalAnalyzer::getNextToken() {
//    // Skip any leading whitespaces
//    skipWhitespaces();
//
//    // If we've reached the end of input, return an empty token
//    if (currentPosition >= input.length())
//        return { "", "" };
//
//    // Store the best match found
//    std::string bestMatchText;
//    std::string bestMatchType;
//    size_t bestMatchLength = 0;
//
//    // Try to match the longest valid token starting from current position
//    for (size_t end = currentPosition + 1; end <= input.length(); ++end) {
//        std::string currentSubstring = input.substr(currentPosition, end - currentPosition);
//        std::string currentMatchType;
//        bool foundMatch = false;
//
//        // Check if the current substring matches any token DFA
//        for (auto& [tokenType, dfa] : tokenDFAs) {
//            if (matchesTokenDFA(dfa, currentSubstring)) {
//                // If this is a longer match or a match with a token defined earlier
//                if (currentSubstring.length() > bestMatchLength ||
//                    (currentSubstring.length() == bestMatchLength &&
//                        std::find(tokenDFAs.begin(), tokenDFAs.end(), std::make_pair(tokenType, dfa)) <
//                        std::find(tokenDFAs.begin(), tokenDFAs.end(), std::make_pair(bestMatchType, tokenDFAs.at(bestMatchType))))) {
//                    bestMatchText = currentSubstring;
//                    bestMatchType = tokenType;
//                    bestMatchLength = currentSubstring.length();
//                }
//                foundMatch = true;
//            }
//        }
//
//        // If no match is found, break the loop
//        if (!foundMatch)
//            break;
//    }
//
//    // If no match was found, throw an error
//    if (bestMatchText.empty())
//        throw std::runtime_error("No valid token found at position: " + std::to_string(currentPosition));
//
//    // Update current position and return the best match
//    currentPosition += bestMatchText.length();
//    return { bestMatchText, bestMatchType };
//}

// Check if a substring matches a token's DFA
bool LexicalAnalyzer::matchesTokenDFA(DFA& dfa, std::string& substring) {
    auto table = dfa.getTable();
	auto startClosure = dfa.getStartClosure();
	auto terminalClosures = dfa.getTerminalClosures();

	auto currentState = startClosure;
    
    // Traverse through the substring using the DFA
    for (char c : substring) {
        // If no transition exists for the current character, return false
        if (table[currentState].count(c) == 0)
            return false;

        // Move to the next state
        currentState = table[currentState][c];
    }

	// Return true if the final state is a terminal state
	return terminalClosures.contains(currentState);
}

// Skip leading whitespace characters
void LexicalAnalyzer::skipWhitespaces() {
	for (; currentPosition < input.length() && std::isspace(input[currentPosition]); ++currentPosition);
}