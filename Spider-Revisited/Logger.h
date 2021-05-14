/*This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)*/
#pragma once

class Logger
{
public:
    static void logInfo(const char* message, const char* file, unsigned int line);
    static void logWarn(const char* message, const char* file, unsigned int line);
    static void logFatal(const char* message, const char* file, unsigned int line);
    static void logDebug(const char* message, const char* file, unsigned int line);
};