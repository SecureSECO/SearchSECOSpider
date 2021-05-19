/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Spider.h"
#include <iostream>
#include <map>

// TODO: Add combining of exit codes.
AuthorData Spider::download(std::string url, std::string filePath, std::string branch)
{
	int result = downloadSource(url, filePath, branch);
	// Check if downloading source failed.
	if (result != 0)
	{
		std::cout << "Failed to clone '" << url << "'.";
		return AuthorData();
	}

	downloadMetaData(url, filePath);

	AuthorData output = downloadAuthor(url, filePath);

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