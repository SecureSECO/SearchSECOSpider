/*This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)*/
#include "loguru.hpp"
#include "Logger.h"
#include <stdlib.h> 
void Logger::logInfo(const char* message, const char* file, unsigned int line)
{
    loguru::log(loguru::Verbosity_INFO, file, line, message);
}
void Logger::logWarn(const char* message, const char* file, unsigned int line)
{
    loguru::log(loguru::Verbosity_WARNING, file, line, message);
}
void Logger::logFatal(const char* message, const char* file, unsigned int line)
{
    loguru::log(loguru::Verbosity_ERROR, file, line, message);
    exit(0);
}
void Logger::logDebug(const char* message, const char* file, unsigned int line)
{
    loguru::log(loguru::Verbosity_1, file, line, message);
}