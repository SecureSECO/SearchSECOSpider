﻿/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include <iostream>
#include <regex>
#include "ExecuteCommand.h"
#include <sstream>
#include "GitSpider.h"
#include "RunSpider.h"
#include "Spider.h"
#include "Logger.h"

AuthorData RunSpider::runSpider(std::string const &url, std::string const &filePath, int threads,
								std::string const &tag, std::string const &nextTag, std::string const &branch)
{
	loguru::set_thread_name("spider");

	// Delete the folder at filepath, so that git does not throw an error.
	std::cout << "Deleting old files..." << std::endl;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	ExecuteCommand::exec(("rmdir \"" + filePath + "\"/S /Q").c_str());
#else
	ExecuteCommand::exec(("rm -rf " + filePath).c_str());
#endif
	
	// Check which spider to use for link.
	Spider *spider = getSpider(url);
	if (spider == nullptr)
	{
		errno = 1;
		return AuthorData();
	}
	spider->setThreads(threads);
	spider->setParsableExts(EXTS);

	AuthorData output;
	try
	{
		output = spider->download(url, filePath, branch, tag, nextTag);
	}
	catch (int e)
	{
		errno = e;
		return AuthorData();
	}
	delete spider;

	errno = 0;
	return output;
}

bool sortByTimestamp(std::pair<std::string, long long> const &a, std::pair<std::string, long long> const &b)
{
	return a.second < b.second;
}

std::vector<std::pair<std::string, long long>> RunSpider::getTags(std::string const &filePath)
{
	// Get all the tags in the repository.
	std::string command = "cd \"" + filePath + "\" && git tag";
	std::string tagsStr = ExecuteCommand::execOut(command.c_str());

	std::vector<std::pair<std::string, long long>> tags;
	
	std::stringstream ss(tagsStr);
	std::string to;

	// Get UNIX timestamp for each tag.
	while (std::getline(ss, to, '\n'))
	{
		command = "cd \"" + filePath + "\" && git show -1 -s --format=%ct " + to;
		std::string timeStampStr = ExecuteCommand::execOut(command.c_str());

		// Add to pair
		long long timeStamp = stoll(timeStampStr.substr(0, timeStampStr.length() - 1));
		tags.push_back(std::make_pair(to, timeStamp));
	}

	// Sort the tags by timestamp.
	std::sort(tags.rbegin(), tags.rend(), sortByTimestamp);

	return tags;
}

Spider *RunSpider::getSpider(std::string const &url)
{
	// Check if link belongs to github or gitlab.
	if (std::regex_match(url, std::regex("https://(www\\.)?(github|gitlab)\\.com([^ ]*)")))
	{
		return new GitSpider();
	}

	return nullptr;
}
