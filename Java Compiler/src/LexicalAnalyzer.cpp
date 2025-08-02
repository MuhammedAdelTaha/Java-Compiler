#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(
    const std::string& inputText,
    DFA& dfa,
	std::set<std::string>& keywords,
	std::set<std::string>& punctuations,
    std::map<std::string, int>& tokensPrecedence,
    std::map<std::string, std::set<EpsilonClosure>>& regexEpsilonClosures
)
	: m_CurrentPosition(0), m_InputText(inputText), m_StartClosure(dfa.getStartClosure()), m_TerminalClosures(dfa.getTerminalClosures()), 
	m_Table(dfa.getTable()), m_Keywords(keywords), m_Punctuations(punctuations), m_TokensPrecedence(tokensPrecedence), 
    m_RegexEpsilonClosures(regexEpsilonClosures)
{
}

// Main method to get the next token from the input.
std::pair<std::string, std::string> LexicalAnalyzer::getNextToken()
{
    // Skip any leading whitespaces.
    skipWhitespaces();

    // If we've reached the end of input, return an empty token.
    if (m_CurrentPosition >= m_InputText.length())
        return { "$", "$" };

    if (m_Punctuations.contains(m_InputText.substr(m_CurrentPosition, 1)))
    {
		m_CurrentPosition++;
        std::string punctuation = m_InputText.substr(m_CurrentPosition - 1, 1);
        return { punctuation, punctuation };
    }

    // Store the best match found
	std::string charSequence;
    std::string bestMatchText;
    std::set<std::string> bestMatchTokens;
    EpsilonClosure currentClosure = m_StartClosure;

	size_t temp = m_CurrentPosition;
    // Try to match the longest valid token starting from current position
    for (size_t start = m_CurrentPosition; start < m_InputText.length(); start++)
    {
		char input = m_InputText[start];
		charSequence += input;
		if (m_Keywords.contains(charSequence))
		{
			m_CurrentPosition = start + 1;
			return { charSequence, charSequence };
		}

		// If no transition exists for the current character, break the loop
		if (m_Table[currentClosure].count(input) == 0)
			break;

		currentClosure = m_Table[currentClosure][input];
		if (!m_TerminalClosures.contains(currentClosure))
			continue;

		bestMatchText = m_InputText.substr(m_CurrentPosition, start - m_CurrentPosition + 1);

        bestMatchTokens.clear();
        for (auto [token, closure] : m_RegexEpsilonClosures)
            if (closure.count(currentClosure))
                bestMatchTokens.insert(token);
    }

    // If no match was found, throw an error
    if (bestMatchText.empty())
    {
        std::cout << "No valid token found at position: " + std::to_string(m_CurrentPosition) << std::endl;
        m_CurrentPosition = temp + 1;
        return { "error", "error" };
    }
    
    std::string bestMatchToken;
	for (auto token : bestMatchTokens)
		if (bestMatchToken.empty() || m_TokensPrecedence[token] > m_TokensPrecedence[bestMatchToken])
			bestMatchToken = token;

    // Update current position and return the best match
    m_CurrentPosition += bestMatchText.length();
    return { bestMatchText, bestMatchToken };
}

// Skip leading whitespace characters
void LexicalAnalyzer::skipWhitespaces() 
{
	for (; m_CurrentPosition < m_InputText.length() && std::isspace(m_InputText[m_CurrentPosition]); m_CurrentPosition++);
}