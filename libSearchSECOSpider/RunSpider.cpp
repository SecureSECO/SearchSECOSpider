/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Spider includes.
#include "ExecuteCommand.h"
#include "GitSpider.h"
#include "Logger.h"
#include "RunSpider.h"

// External includes.
#include <iostream>
#include <regex>
#include <sstream>


/*std::tuple<AuthorData, std::string, std::vector<std::string>> RunSpider::runSpider(
	std::string const &url,
	std::string const &filePath, 
	int threads, 
	std::string const &tag, 
	std::string nextTag, 
	std::string const &branch)
{
	loguru::set_thread_name("spider");
	std::string entryLog = "Downloading project source files";
	if (!branch.empty())
	{
		entryLog += " from the " + branch + " branch";
	}
	Logger::logDebug(entryLog, __FILE__, __LINE__);

	// Delete the folder at filepath, so that git does not throw an error.
	Logger::logDebug("Deleting old files from ./" + filePath + "/", __FILE__, __LINE__);
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
		return std::make_tuple(AuthorData(), "", std::vector<std::string>());
	}

	// Set up spider.
	spider->setThreads(threads);
	spider->setParsableExts(EXTS);

	// Set nextTag to HEAD if no tag was specified.
	if (nextTag == "")
	{
		nextTag = "HEAD";
	}

	// Try to download authordata.
	AuthorData authordata;
	try
	{
		authordata = spider->download(url, filePath, branch, tag, nextTag);
	}
	catch (int e)
	{
		errno = e;
		return std::make_tuple(AuthorData(), "", std::vector<std::string>());
	}

	// Get additional info from repository.
	std::string commitHash = getCommitHash(nextTag, filePath);
	std::vector<std::string> unchangedFiles = spider->getUnchangedFiles();

	// Cleanup spider.
	delete spider;

	// Prepare output.
	auto output = std::make_tuple(authordata, commitHash, unchangedFiles);

	Logger::logDebug("Download successful",
		__FILE__, __LINE__);

	errno = 0;
	return output;
}*/

Spider* RunSpider::setupSpider(std::string const &url, int threads)
{
	// Check which spider to use for link.
	Spider* spider = getSpider(url);
	if (spider == nullptr)
	{
		errno = 1;
		return nullptr;
	}

	// Set up spider.
	spider->setThreads(threads);
	spider->setParsableExts(EXTS);

	return spider;
}

void RunSpider::downloadRepo(Spider *spider, std::string const &url, std::string const &filePath,
							 std::string const &branch)
{
	errno = 0;
	loguru::set_thread_name("spider");
	std::string entryLog = "Downloading project source files";
	if (!branch.empty())
	{
		entryLog += " from the " + branch + " branch";
	}
	Logger::logInfo(entryLog, __FILE__, __LINE__);

	// Delete the folder at filepath, so that git does not throw an error.
	Logger::logDebug("Deleting old files from ./" + filePath + "/", __FILE__, __LINE__);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	ExecuteCommand::exec(("rmdir \"" + filePath + "\"/S /Q").c_str());
#else
	ExecuteCommand::exec(("rm -rf " + filePath).c_str());
#endif

	// Try to clone repository.
	try
	{
		spider->download(url, filePath, branch);
	}
	catch (int e)
	{
		errno = e;
		return;
	}

	Logger::logInfo("Download successful", __FILE__, __LINE__);
	return;
}

std::vector<std::string> RunSpider::updateVersion(Spider *spider, std::string const &filePath, std::string const &prevTag,
												  std::string const &newTag)
{
	std::vector<std::string> res;

	// Try to update repository.
	try
	{
		res = spider->update(filePath, prevTag, newTag);
	}
	catch (int e)
	{
		errno = e;
		res = std::vector<std::string>();
	}
	return res;
}

AuthorData RunSpider::getAuthors(Spider *spider, std::string const &filePath)
{
	AuthorData res;

	// Try to extract authors.
	try
	{
		res = spider->downloadAuthor(filePath);
	}
	catch (int e)
	{
		errno = e;
		res = AuthorData();
	}
	return res;
}

bool sortByTimestamp(std::tuple<std::string, long long, std::string> const &a,
                     std::tuple<std::string, long long, std::string> const &b)
{
    return std::get<1>(a) < std::get<1>(b);
}

std::vector<std::tuple<std::string, long long, std::string>> RunSpider::getTags(std::string const &filePath)
{
	// Get all the tags in the repository.
	std::string command = "cd \"" + filePath + "\" && git tag";
	std::string tagsStr = ExecuteCommand::execOut(command.c_str());

	std::vector<std::tuple<std::string, long long, std::string>> tags;
	
	std::stringstream ss(tagsStr);
	std::string to;

	// Get UNIX timestamp for each tag.
	while (std::getline(ss, to, '\n'))
	{
		command = "cd \"" + filePath + "\" && git show -1 -s --format=%ct " + to;
		std::string timeStampStr = ExecuteCommand::execOut(command.c_str());

		long long timeStamp = stoll(timeStampStr);
		std::string commitHash = getCommitHash(to, filePath);
		tags.push_back(std::make_tuple(to, timeStamp, commitHash));
	}

	// Sort the tags by timestamp.
	std::sort(tags.begin(), tags.end(), sortByTimestamp);

	return tags;
}

std::string RunSpider::getCommitHash(std::string const &tag, std::string const &filePath)
{
	std::string command = "cd \"" + filePath + "\" && git rev-list -n 1 " + tag;
	std::string hash = ExecuteCommand::execOut(command.c_str());
	// Last character is newline, so remove that before returning.
	return hash.substr(0, hash.length() - 1);
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
