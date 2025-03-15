#include "Logger.h"
#include <iostream>

void Logger::logInfo(const std::string& message) {
    std::cout << "[INFO]: " << message << std::endl;
}

void Logger::logError(const std::string& message) {
    std::cerr << "[ERROR]: " << message << std::endl;
}