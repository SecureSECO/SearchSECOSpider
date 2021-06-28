/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Spider includes.
#include "ErrorSpider.h"
#include "ExecuteCommand.h"
#include "Logger.h"

// External includes.
#include <array>
#include <memory>

#define bufferSize 1024


void ExecuteCommandObj::exec(const char* cmd)
{
	std::array<char, bufferSize> buffer = std::array<char, bufferSize>();

	// Open console to call git commands.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
#else
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
#endif
	if (!pipe)
	{
		Logger::logFatal(Error::getErrorMessage(ErrorType::PipeOpenFailed),
			__FILE__, __LINE__, (int) ErrorType::PipeOpenFailed);
		throw 1;
	}
	// Amount of data read, is less then bufferSize if output ends.
	size_t bytesRead;

	// Read until there is nothing more to read, insert chunks into stream.

	while ((bytesRead = fread(buffer.data(), 1, bufferSize, pipe.get())) > 0);
}

std::string ExecuteCommandObj::execOut(const char *cmd)
{
	std::array<char, bufferSize> buffer = std::array<char, bufferSize>();
	std::string res;
	// Open console to call git commands.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
#else
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
#endif
	if (!pipe)
	{
		Logger::logFatal(Error::getErrorMessage(ErrorType::PipeOpenFailed),
			__FILE__, __LINE__, (int)ErrorType::PipeOpenFailed);
		throw 1;
	}
	// Amount of data read, is less then bufferSize if output ends.
	size_t bytesRead;

	// Read until there is nothing more to read, insert chunks into stream.
	while ((bytesRead = fread(buffer.data(), 1, bufferSize, pipe.get())) > 0) 
	{
		std::string str = buffer.data();
		res += str.erase(bytesRead, str.size() - bytesRead);
	}
	return res;
}
