/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

#include "loguru/loguru.hpp"
#include <string>

class Logger
{
private:
	/// <summary>
	/// Puts the error code before a message.
	/// </summary>
	/// <param name="message">The text message.</param>
	/// <param name="code">The error code.</param>
	/// <returns>A string consisting of an error code and then a message.</returns>
	static std::string getErrorMessage(std::string const &message, int code);

public:
	/// <summary>
	/// Log an info message.
	/// </summary>
	/// <param name="message">The message.</param>
	/// <param name="file">The file in which this function was called from.</param>
	/// <param name="line">The line number from which this function was called from.</param>
	static void logInfo(std::string const &message, const char* file, unsigned int line);

	/// <summary>
	/// Log a warning message.
	/// </summary>
	/// <param name="message">The message.</param>
	/// <param name="file">The file in which this function was called from.</param>
	/// <param name="line">The line number from which this function was called from.</param>
	static void logWarn(std::string const &message, const char *file, unsigned int line);

	/// <summary>
	/// Log an error message.
	/// </summary>
	/// <param name="message">The message.</param>
	/// <param name="file">The file in which this function was called from.</param>
	/// <param name="line">The line number from which this function was called from.</param>
	/// <param name="code">The relevant error code.</param>
	static void logFatal(std::string const &message, const char* file, unsigned int line, int code);

	/// <summary>
	/// Log a debug message.
	/// </summary>
	/// <param name="message">The message.</param>
	/// <param name="file">The file in which this function was called from.</param>
	/// <param name="line">The line number from which this function was called from.</param>
	static void logDebug(std::string const &message, const char* file, unsigned int line);
};
