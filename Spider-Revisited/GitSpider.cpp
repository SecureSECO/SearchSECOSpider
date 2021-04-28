/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "GitSpider.h"
#include <iostream>
#include <filesystem>
#include <thread>

#define MAX_THREADS 16
#define FILES_PER_CALL 16

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
    std::map<std::string, std::vector<CodeBlock>> output = parseBlameData(repoPath);
	return 0;
}

void GitSpider::blameFiles(std::string repoPath, std::vector<std::string> filePaths)
{
	// Find path to file inside the Repo folder.
	std::vector<std::string> outPaths;
	for (int i = 0; i < filePaths.size(); i++)
	{
		filePaths[i] = filePaths[i].substr(repoPath.length() + 1);
		outPaths.push_back(filePaths[i] + ".meta");
	}
	Git::blameToFile(repoPath, filePaths, outPaths);
}

void GitSpider::singleThread(std::string repoPath, int &blamedPaths, const int &totalPaths, std::queue<std::string> &files, std::mutex &queueLock)
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
		std::vector<std::string> blame;
		// Add FILES_PER_CALL files or the amount that is remaining in the queue.
		for (int i = 0; i < FILES_PER_CALL && files.size() > 0; i++)
		{
			blame.push_back(files.front());
			files.pop();
		}
		queueLock.unlock();
		blameFiles(repoPath, blame);
		cmdLock.lock();
		// Update progress.
		blamedPaths+=blame.size();
		std::cout << '\r' << "Blaming files: " << (100 * blamedPaths) / totalPaths << "% ("
			<< blamedPaths << '/' << totalPaths << ')';
		cmdLock.unlock();
	}
}

std::map<std::string, std::vector<CodeBlock>> GitSpider::parseBlameData(std::string repoPath)
{
    // Thread-safe map (with lock).
    std::map<std::string, std::vector<CodeBlock>> authorData;
    std::mutex mapLock;
    auto dirIter = std::filesystem::recursive_directory_iterator(repoPath);

	// Variables for displaying progress.
    int processedPaths = 0;
    const int totalPaths = std::count_if(begin(dirIter), end(dirIter), [&repoPath](auto &path) {
        return !((path.path()).string().rfind(repoPath + "\\.git", 0) == 0) && path.is_regular_file();
    });

    // Loop over all files.
    for (const auto &path : std::filesystem::recursive_directory_iterator(repoPath))
    {
        std::string s = (path.path()).string();
        if (!(s.rfind(repoPath + "\\.git", 0) == 0) && path.is_regular_file() && path.path().extension() == ".meta")
        {
            authorData.insert(std::pair<std::string, std::vector<CodeBlock>>(s, Git::getBlameData(s)));

            processedPaths ++;
            std::cout << '\r' << "Processing blame data: " << (100 * processedPaths) / totalPaths << "% (" << processedPaths << '/'
                      << totalPaths << ')';
        }
    }

	std::cout << ", done." << std::endl;
    return authorData;
}
