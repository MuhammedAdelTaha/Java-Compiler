#include "Utilities.h"
#include <sstream>

std::vector<std::string> splitWords(const std::string& str) 
{
    std::vector<std::string> words;
    std::istringstream iss(str);
    std::string word;

    // Use stream extraction to automatically handle multiple whitespaces.
    while (iss >> word)
        words.push_back(word);

    return words;
}