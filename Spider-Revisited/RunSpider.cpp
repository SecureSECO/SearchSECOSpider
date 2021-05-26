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

AuthorData RunSpider::runSpider(std::string const &url, std::string const &filePath, int threads,
								int &code, std::string const &branch)
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
	spider->setThreads(threads);

	AuthorData output;
	try
	{
		output = spider->download(url, filePath, branch);
	}
	catch (int e)
	{
		code = e;
	}
	delete spider;
	return output;
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