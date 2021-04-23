/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "GitSpider.h"
#include "Git.h"
#include <iostream>
#include <filesystem>
#include <thread>
#include <mutex>
#include <queue>

#define MAX_THREADS 16

// Global locks.
std::mutex cmdLock;

// TODO: Return code.
int GitSpider::downloadSource(std::string url, std::string filePath)
{
	Git::clone(url, filePath);
	return 0;
}

int GitSpider::downloadMetaData(std::string url, std::string repoPath)
{
	return 0;
}

// TODO: Return code.
int GitSpider::downloadAuthor(std::string url, std::string repoPath)
{
	std::vector<std::thread> threads;

	// Thread-safe queue (with lock).
	std::queue<std::string> files;
	std::mutex queueLock;
	auto dirIter = std::filesystem::recursive_directory_iterator(repoPath);

	// Variables for displaying progress.
	int blamedPaths = 0;
	const int totalPaths = std::count_if(begin(dirIter), end(dirIter), [&repoPath](auto& path)
		{ return !((path.path()).string().rfind(repoPath + "\\.git", 0) == 0) && path.is_regular_file(); });

	// Loop over all files.
	for (const auto& path : std::filesystem::recursive_directory_iterator(repoPath))
	{
		std::string s = (path.path()).string();
		if (!(s.rfind(repoPath + "\\.git", 0) == 0) && path.is_regular_file()) {
			files.push(s);
		}
	}

	// Construct threads to process the queue.
	for (int i = 0; i < MAX_THREADS; i++) {
		threads.push_back(std::thread(&GitSpider::singleThread, this, repoPath, std::ref(blamedPaths), std::ref(totalPaths), std::ref(files), std::ref(queueLock)));
	}

	// Wait on threads to finish.
	for (auto& th : threads) {
		th.join();
	}
	std::cout << ", done." << std::endl;
	return 0;
}

void GitSpider::downloadSingleAuthor(std::string repoPath, std::string filePath)
{
	// Find path to file inside the Repo folder.
	std::string relPath = filePath.substr(repoPath.length() + 1);
	std::string outPath = relPath + ".meta";
	Git::blameToFile(repoPath, relPath, outPath);
}

void GitSpider::singleThread(std::string repoPath, int &blamedPaths, const int &totalPaths, std::queue<std::string> &files, std::mutex &queueLock)
{
	while (true) 
	{
		queueLock.lock();
		if (files.size() <= 0)
		{
			queueLock.unlock();
			return;
		}
		std::string s = files.front();
		files.pop();
		queueLock.unlock();
		downloadSingleAuthor(repoPath, s);
		cmdLock.lock();
		// Update progress.
		blamedPaths++;
		std::cout << '\r' << "Blaming files: " << (100 * blamedPaths) / totalPaths << "% ("
			<< blamedPaths << '/' << totalPaths << ')';
		cmdLock.unlock();
	}
}
