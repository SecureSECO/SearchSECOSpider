/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Spider includes.
#include "Git.h"
#include "Spider.h"

// External includes.
#include <filesystem>
#include <mutex>
#include <queue>


class GitSpider : public Spider
{
private:
	Git git;

	/// <summary>
	/// Implements the abstract downloadSource method from the Spider class.
	/// </summary>
	/// <param name="url"> Url to repository to download. </param>
	/// <param name="filePath"> Local path where to store the repository. </param>
	/// <param name="branch"> Branch of the source to download. </param>
	/// <param name="tag"> Tag that came before nextTag, used to calculate differences. </param>
	/// <param name="nextTag"> Tag to download. Pass HEAD to download most recent version. </param>
	/// <returns></returns>
	int downloadSource(std::string const &url, std::string const &filePath, std::string const &branch,
						std::string const &tag, std::string const &nextTag) override;

	/// <summary>
	/// Implements the abstract downloadAuthor method from the Spider class.
	/// </summary>
	/// <param name="filePath"> Local location of the repository. </param>
	/// <returns> AuthorData of the files in the repository. </returns>
	AuthorData downloadAuthor(std::string const &repoPath) override;

	/// <summary>
	/// Run on a single thread, takes files from the queue and blames these sequentially.
	/// </summary>
	/// <param name="repoPath"> Path to local repository. </param>
	/// <param name="blamedPaths"> Amount of paths blamed. </param>
	/// <param name="totalPaths"> Total paths that need to be blamed. </param>
	/// <param name="files"> Queue containing all files to be blamed. </param>
	/// <param name="queueLock"> Lock to prevent racing conditions. </param>
	void singleThread(std::string const &repoPath, int &blamedPaths, const int &totalPaths,
			std::queue<std::filesystem::path> &files, std::mutex &queueLock);

	/// <summary>
	/// Parses blame data from all .meta files in a directory.
	/// </summary>
	/// <param name="repoPath"> Directory containing files to parse. </param>
	/// <returns> Map with original file path (without .meta) as key and CodeBlocks as value. </returns>
	AuthorData parseBlameData(std::string const &repoPath);

public:
	void setParsableExts(std::string const &exts) override;

	std::vector<std::string> getUnchangedFiles() override;
};
