#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    static void logInfo(const std::string& message);
    static void logError(const std::string& message);
};

#endif