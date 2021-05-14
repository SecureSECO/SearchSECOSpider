/*This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)*/
#pragma once
#include <string>

class Logger
{
private:
    static std::string getMessage(const char* message, int code);
public:
    static void logInfo(const char* message, const char* file, unsigned int line, int code);
    static void logWarn(const char* message, const char* file, unsigned int line, int code);
    static void logFatal(const char* message, const char* file, unsigned int line, int code);
    static void logDebug(const char* message, const char* file, unsigned int line, int code);
};