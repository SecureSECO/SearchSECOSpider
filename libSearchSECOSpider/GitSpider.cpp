/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Filesystem.h"
#include "GitSpider.h"
#include <iostream>
#include <filesystem>
#include <thread>

#define FILES_PER_CALL 16

// Global locks.
std::mutex cmdLock;

int GitSpider::downloadSource(std::string const &url, std::string const &filePath, std::string const &branch,
							  std::string const &tag, std::string const &nextTag)
{
	return git.clone(url, filePath, branch, parsableExts, tag, nextTag);
}

AuthorData GitSpider::downloadAuthor(std::string const &repoPath)
{
	std::vector<std::thread> threads;

	// Thread-safe queue (with lock).
	auto pred = [repoPath](std::filesystem::directory_entry path)
	{
		std::string str = path.path().string();
		return !(str.rfind(repoPath + "\\.git", 0) == 0 || str.rfind(repoPath + "/.git", 0) == 0)
				&& Filesystem::isRegularFile(str);
	};
	std::queue<std::filesystem::path> files = Filesystem::getFilepaths(repoPath, pred);
	std::mutex queueLock;

	// Variables for displaying progress.
	int blamedPaths = 0;
	const int totalPaths = files.size();

	// Construct threads to process the queue.
	for (int i = 0; i < threadsCount; i++)
	{
		threads.push_back(std::thread(&GitSpider::singleThread, this, repoPath, std::ref(blamedPaths),
			std::ref(totalPaths), std::ref(files), std::ref(queueLock)));
	}

	// Wait on threads to finish.
	for (auto& th : threads) {
		th.join();
	}
	std::cout << ", done." << std::endl;
	AuthorData output = parseBlameData(repoPath);
	return output;
}

void GitSpider::singleThread(std::string const &repoPath, int &blamedPaths, const int &totalPaths,
							 std::queue<std::filesystem::path> &files, std::mutex &queueLock)
{
	while (true) 
	{
		// Lock the queue.
		queueLock.lock();
		if (files.size() <= 0)
		{
			queueLock.unlock();
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
		
		cmdLock.lock();
		// Update progress.
		blamedPaths += filesCount;
		std::cout << '\r' << "Blaming files: " << (100 * blamedPaths) / totalPaths <<
			"% (" << blamedPaths << '/' << totalPaths << ')';
		cmdLock.unlock();
	}
}

AuthorData GitSpider::parseBlameData(std::string const &repoPath)
{
	// Thread-safe map (with lock).
	AuthorData authorData;
	std::mutex mapLock;

	// Variables for displaying progress.
	auto pred = [repoPath](std::filesystem::directory_entry path)
	{
		std::string str = path.path().string();
		return !(str.rfind(repoPath + "\\.git", 0) == 0 || str.rfind(repoPath + "/.git", 0) == 0)
			&& Filesystem::isRegularFile(str) && path.path().extension() == ".meta";
	};
	auto paths = Filesystem::getFilepaths(repoPath, pred);
	int processedPaths = 0;
	const int totalPaths = paths.size();

	// Loop over all files.
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

		// Report progress.
		processedPaths++;
		std::cout << '\r' << "Processing blame data: " << (100 * processedPaths) / totalPaths << "% ("
					<< processedPaths << '/' << totalPaths << ')';
	}

	std::cout << ", done." << std::endl;
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