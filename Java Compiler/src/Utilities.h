#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

std::vector<std::string> splitWords(const std::string& str);
std::string readFileToString(const std::string& filePath);