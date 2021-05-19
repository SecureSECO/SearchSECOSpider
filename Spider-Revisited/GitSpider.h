/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include "Spider.h"
#include "Git.h"
#include <mutex>
#include <queue>
#include <map>

class GitSpider : public Spider
{
private:
	Git *git;

	/// <summary>
	/// Implements the abstract downloadSource method from the Spider class.
	/// </summary>
	int downloadSource(std::string url, std::string filePath, std::string branch) override;

	/// <summary>
	/// Implements the abstract downloadMetaData method from the Spider class.
	/// </summary>
	int downloadMetaData(std::string url, std::string repoPath) override;

	/// <summary>
	/// Implements the abstract downloadAuthor method from the Spider class.
	/// </summary>
	AuthorData downloadAuthor(std::string url, std::string repoPath) override;

	/// <summary>
	/// Downloads author data for a single given file. Thread-safe.
	/// </summary>
	void blameFiles(std::string repoPath, std::vector<std::string> filePaths);

	/// <summary>
	/// Run on a single thread, takes files from the queue and blames these sequentially.
	/// </summary>
	void singleThread(std::string repoPath, int &blamedPaths, const int &totalPaths, std::queue<std::string> &files, std::mutex &queueLock);

	/// <summary>
	/// Parses blame data from all .meta files in a directory.
	/// </summary>
	/// <param name="repoPath"> Directory containing files to parse. </param>
	/// <returns> Map with original file path (without .meta) as key and CodeBlocks as value. </returns>
	AuthorData parseBlameData(std::string repoPath);
};

