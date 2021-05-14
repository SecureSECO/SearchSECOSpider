/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include <iostream>
#include "RunSpider.h"
#include "Spider.h"
#include "GitSpider.h"
#include "ExecuteCommand.h"

AuthorData RunSpider::runSpider(std::string url, std::string filePath, int &code)
{
	// Delete the folder at filepath, so that git does not throw an error.
	std::cout << "Deleting old files..." << std::endl;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	ExecuteCommand::exec(("rmdir \"" + filePath + "\"/S /Q").c_str());
#else
	ExecuteCommand::exec(("rm -rf " + filePath).c_str());
#endif

	// For now, default to the Git Spider.
	Spider *spider = new GitSpider();
	AuthorData output;
	try
	{
		output = spider->download(url, filePath);
	}
	catch (int e)
	{
		code = e;
	}
	delete spider;
	return output;
}
