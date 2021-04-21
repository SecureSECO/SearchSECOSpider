#include "ExecuteCommand.h"
#include <array>
#include <iostream>

#define bufferSize 1024

void ExecuteCommand::exec(const char* cmd)
{
	std::array<char, bufferSize> buffer = std::array<char, bufferSize>();

	// Open console to call git commands
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
#else
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
#endif
	if (!pipe)
	{
		throw std::runtime_error("popen() failed!");
	}
	// Amount of data read, is less then bufferSize if output ends
	size_t bytesRead;

	// Read until there is nothing more to read, insert chunks into stream

	while ((bytesRead = fread(buffer.data(), 1, bufferSize, pipe.get())) > 0);
}
