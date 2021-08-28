/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Spider includes.
#include "Spider.h"

void Spider::setThreads(int threads)
{
	if (threads < 1)
	{
		throw "Can't set amount of threads lower than 1.";
	}

	threadsCount = threads;
}

int Spider::getThreads()
{
	return threadsCount;
}

void Spider::setParsableExts(std::string const &exts)
{
	parsableExts = exts;
}
