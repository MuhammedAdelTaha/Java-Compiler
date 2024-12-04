#include "Utilities.h"

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

std::string readFileToString(const std::string& filePath)
{
    std::ifstream file(filePath);
    std::string line;

    if (file.peek() == 0xEF)
    {
        char bom[3];
        file.read(bom, 3);
        if (!(bom[0] == (char)0xEF && bom[1] == (char)0xBB && bom[2] == (char)0xBF))
            file.seekg(0);
    }

	std::ostringstream oss;
	while (std::getline(file, line))
	{
		// Remove leading and trailing whitespaces.
		line.erase(0, line.find_first_not_of(" \t"));
		line.erase(line.find_last_not_of(" \t") + 1);

		if (line.empty())
			continue;

		oss << line << '\n';
	}

	return oss.str();
}