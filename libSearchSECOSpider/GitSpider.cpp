/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Spider includes.
#include "GitSpider.h"
#include "Filesystem.h"
#include "Logger.h"

// External includes.
#include <iostream>
#include <sstream>
#include <thread>

#define FILES_PER_CALL 16

void GitSpider::download(std::string const &url, std::string const &filePath, std::string const &branch)
{
	return git.clone(url, filePath, branch, parsableExts);
}

std::vector<std::string> GitSpider::update(std::string const &filePath, std::string const &prevTag, 
														std::string const &newTag, std::vector<std::string> prevUnchangedFiles)
{
	return git.getDifference(prevTag, newTag, filePath, prevUnchangedFiles);
}

void GitSpider::switchVersion(std::string const &filePath, std::string const &tag)
{
	git.changeTag(tag, filePath);
}

AuthorData GitSpider::downloadAuthor(std::string const &repoPath)
{
	std::vector<std::thread> threads;

	// Thread-safe queue (with lock).
	auto pred = [repoPath](std::filesystem::directory_entry path) {
		std::string str = path.path().string();
		return !(str.rfind(repoPath + "\\.git", 0) == 0 || str.rfind(repoPath + "/.git", 0) == 0) &&
			   Filesystem::isRegularFile(str);
	};
	std::queue<std::filesystem::path> files = Filesystem::getFilepaths(repoPath, pred);
	std::mutex queueLock;

	// Variables for displaying progress.
	const int totalPaths = files.size();

	// Construct threads to process the queue.
	Logger::logInfo("Blaming and processing " + std::to_string(totalPaths) + " files", __FILE__, __LINE__);
	for (int i = 0; i < threadsCount; i++)
	{
		threads.push_back(std::thread(&GitSpider::singleThread, this, repoPath, std::ref(totalPaths), std::ref(files),
									  std::ref(queueLock)));
	}

	// Wait on threads to finish.
	for (auto &th : threads)
	{
		th.join();
	}
	Logger::logDebug("Finished blaming files", __FILE__, __LINE__);

	AuthorData output = parseBlameData(repoPath);
	return output;
}

void GitSpider::singleThread(std::string const &repoPath, const int &totalPaths,
							 std::queue<std::filesystem::path> &files, std::mutex &queueLock)
{
	std::stringstream ss;
	ss << std::this_thread::get_id();

	loguru::set_thread_name(("spider@" + ss.str()).c_str());

	while (true)
	{
		int filesc = files.size();
		Logger::logDebug(std::to_string(filesc) + " files left", __FILE__, __LINE__);
		// Lock the queue.
		queueLock.lock();
		if (filesc <= 0)
		{
			queueLock.unlock();

			loguru::set_thread_name("spider");
			return;
		}
		std::vector<std::string> blameFiles;
		// Add FILES_PER_CALL files or the amount that is remaining in the queue.
		for (int i = 0; i < FILES_PER_CALL && files.size() > 0; i++)
		{
			blameFiles.push_back(files.front().make_preferred().string());
			files.pop();
		}
		int filesCount = blameFiles.size();
		queueLock.unlock();

		// Blame files with git.
		git.blameFiles(repoPath, blameFiles);
	}
}

AuthorData GitSpider::parseBlameData(std::string const &repoPath)
{
	// Thread-safe map (with lock).
	AuthorData authorData;
	std::mutex mapLock;

	// Variables for displaying progress.
	auto pred = [repoPath](std::filesystem::directory_entry path) {
		std::string str = path.path().string();
		return !(str.rfind(repoPath + "\\.git", 0) == 0 || str.rfind(repoPath + "/.git", 0) == 0) &&
			   Filesystem::isRegularFile(str) && path.path().extension() == ".meta";
	};
	auto paths = Filesystem::getFilepaths(repoPath, pred);

	// Loop over all files.
	Logger::logDebug("Processing blame data of " + std::to_string(paths.size()) + " files...", __FILE__, __LINE__);
	while (paths.size() > 0)
	{
		auto path = paths.front();
		paths.pop();

		// Trim string.
		path = path.make_preferred();
		std::string str = path.string().substr(repoPath.length() + 1);
		str.erase(str.length() - 5, 5);

		// Add blame data.
		authorData.insert(std::pair<std::string, std::vector<CodeBlock>>(str, git.getBlameData(path.string())));
	}
	Logger::logDebug("Finished blaming and processing files", __FILE__, __LINE__);

	return authorData;
}

void GitSpider::setParsableExts(std::string const &exts)
{
	int start = 0;
	int end = 0;
	while (end != std::string::npos)
	{
		end = exts.find(" ", start);
		parsableExts += "*" + exts.substr(start, end - start) + " ";
		start = end + 1;
	}
	parsableExts.erase(parsableExts.length() - 1, 1);
}

std::vector<std::string> GitSpider::getUnchangedFiles()
{
	return git.unchangedFiles;
}
