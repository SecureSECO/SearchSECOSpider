/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Spider includes.
#include "Logger.h"

#define BASE 200


void Logger::logDebug(std::string const &message, const char *file, unsigned int line)
{
	loguru::log(loguru::Verbosity_1, file, line, "%s", message.c_str());
}

void Logger::logInfo(std::string const &message, const char *file, unsigned int line)
{
	loguru::log(loguru::Verbosity_INFO, file, line, "%s", message.c_str());
}

void Logger::logWarn(std::string const &message, const char *file, unsigned int line)
{
	loguru::log(loguru::Verbosity_WARNING, file, line, "%s", message.c_str());
}

void Logger::logFatal(std::string const &message, const char *file, unsigned int line, int code)
{
	loguru::log(loguru::Verbosity_ERROR, file, line, "%s", getErrorMessage(message, code).c_str());
}

std::string Logger::getErrorMessage(std::string const &message, int code)
{
	std::string c = std::to_string(BASE + code);
	std::string s = "E" + c + " - " + message;
	return s;
}
