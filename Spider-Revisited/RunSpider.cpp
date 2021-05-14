/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include <iostream>
#include <regex>
#include "ExecuteCommand.h"
#include "GitSpider.h"
#include "RunSpider.h"
#include "Spider.h"

AuthorData RunSpider::runSpider(std::string url, std::string filePath)
{
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
		return AuthorData();
	}

	AuthorData output = spider->download(url, filePath);
	delete spider;
	return output;
}

Spider* RunSpider::getSpider(std::string url)
{
	// Check if link belongs to github or gitlab.
	if (std::regex_match(url, std::regex("https://(www\\.)?(github|gitlab)\\.com([^ ]*)")))
	{
		return new GitSpider();
	}

	return nullptr;
}