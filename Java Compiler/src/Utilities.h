#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

struct Symbol {
    enum class Type {
        Terminal,
        NonTerminal
    };

    Type type;
    std::string name;

    bool operator==(const Symbol& other) const {
        return type == other.type && name == other.name;
    }

    bool operator<(const Symbol& other) const {
        if (type != other.type) {
            return type < other.type;
        }
        return name < other.name;
    }
};

std::string trimString(const std::string& str);
std::string trimStringEdges(const std::string& str);
std::vector<std::string> split(const std::string& str, const char delimeter);
std::string readFileToString(const std::string& filePath);