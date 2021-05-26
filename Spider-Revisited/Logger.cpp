/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/
#include "loguru/loguru.hpp"
#include "Logger.h"
#include <stdlib.h> 
#define BASE 200

void Logger::logInfo(const char* message, const char* file, unsigned int line)
{
    loguru::log(loguru::Verbosity_INFO, file, line, "%s", message);
}
void Logger::logWarn(const char* message, const char* file, unsigned int line)
{
    loguru::log(loguru::Verbosity_WARNING, file, line, "%s", message);
}
void Logger::logFatal(const char* message, const char* file, unsigned int line, int code)
{
    loguru::log(loguru::Verbosity_ERROR, file, line, "%s", getErrorMessage(message, code).c_str());
}
void Logger::logDebug(const char* message, const char* file, unsigned int line)
{
    loguru::log(1, file, line, "%s", message);
}

std::string Logger::getErrorMessage(const char* message, int code)
{
    std::string c = std::to_string(BASE + code);
    std::string s = "E" + c + ": " + message;
    return s;
}