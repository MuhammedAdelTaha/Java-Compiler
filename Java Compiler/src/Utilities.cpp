#include "Utilities.h"
#include <iostream>

std::string trimString(const std::string& str)
{
	std::string trimmedStr = str;
	trimmedStr.erase(std::remove_if(trimmedStr.begin(), trimmedStr.end(), ::isspace), trimmedStr.end());
	return trimmedStr;
}

std::string trimStringEdges(const std::string& str)
{
	std::string trimmedStr = str;
	trimmedStr.erase(0, trimmedStr.find_first_not_of(" \t"));
	trimmedStr.erase(trimmedStr.find_last_not_of(" \t") + 1);
	return trimmedStr;
}

std::vector<std::string> split(const std::string& str, const char delimeter) 
{
	std::string token;
    std::vector<std::string> strs;
	std::istringstream tokenStream(str);
	while (std::getline(tokenStream, token, delimeter))
		strs.push_back(trimStringEdges(token));
	return strs;
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

//int main()
//{
//	std::vector<std::string> strs = split("Hello World, Mohamed, Adel Mahmoud, Taha", ',');
//
//	for (const std::string& str : strs)
//	{
//		std::vector<std::string> tokens = split(str, ' ');
//		for (const std::string& token : tokens)
//			std::cout << token << ',';
//		std::cout << std::endl;
//	}	
//
//	return 0;
//}