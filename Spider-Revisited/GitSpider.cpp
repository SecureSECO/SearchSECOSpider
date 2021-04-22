/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "GitSpider.h"
#include "Git.h"
#include <iostream>
#include <filesystem>

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
	auto dirIter = std::filesystem::recursive_directory_iterator(repoPath);
	// Variables for displaying progress.
	int blamedPaths = 0;
	int totalPaths = std::count_if(begin(dirIter), end(dirIter), [&repoPath](auto& path)
		{ return !((path.path()).string().rfind(repoPath + "\\.git", 0) == 0) && path.is_regular_file(); });

	for (const auto& path : std::filesystem::recursive_directory_iterator(repoPath))
	{
		std::string s = (path.path()).string();
		if (!(s.rfind(repoPath + "\\.git", 0) == 0) && path.is_regular_file()) {
			// Find path to file inside the Repo folder.
			std::string relPath = s.substr(repoPath.length() + 1);
			std::string outPath = relPath + ".meta";
			Git::blameToFile(repoPath, relPath, outPath);
			// Update progress.
			blamedPaths++;
			std::cout << '\r' << "Blaming files: " << (100 * blamedPaths) / totalPaths << "% (" 
				<< blamedPaths << '/' << totalPaths << ')';
		}
	}
	std::cout << ", done." << std::endl;
	return 0;
}
