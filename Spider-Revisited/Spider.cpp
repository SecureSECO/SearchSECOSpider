/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Spider.h"
#include <iostream>
#include <map>

AuthorData Spider::download(std::string const &url, std::string const &filePath, std::string const &branch,
						std::string const &tag, std::string const &nextTag)
{
	int result = downloadSource(url, filePath, branch, tag, nextTag);

	AuthorData output = downloadAuthor(filePath);

	return output;
}

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