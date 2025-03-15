#include "Utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <unordered_map>

std::unordered_map<std::string, std::string> envMap;

void loadEnv() {
    std::ifstream file(".env");
    if (!file) {
        std::cerr << "Error: .env not found" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '=')) {
            std::string value;
            if (std::getline(is_line, value)) {
                envMap[key] = value;
            }
        }
    }
}

std::string getEnv(const std::string& key) {
    if (envMap.empty()) {
        loadEnv();
    }
    if (envMap.find(key) == envMap.end()) {
        std::cerr << "The variable " << key << " was not found in .env" << std::endl;
        return "";
    }
    std::string value = envMap[key];
    value.erase(std::remove_if(value.begin(), value.end(),
        [](unsigned char c) { return c <= 32 || c >= 127; }), value.end());
    return value;
}