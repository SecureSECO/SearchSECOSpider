/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include <string>
#include <vector>

class Git
{
public:
	/// <summary>
	/// clone mirrors the full Clone functionality of the Git system; it requires
	/// a HTTPS link to the Git repository to be cloned, together with the location
	/// where the repository should be cloned into and returns an integer to indicate
	/// the result of the clone operation (success or specific failure).
	/// </summary>
	static int clone(std::string url, std::string filePath);

	/// <summary>
	/// blame mirrors the Blame functionality of the Git system; it requires the path
	/// to a cloned Git Repo and a path to the file to blame, and returns a string 
	/// containing the author data per line.
	/// </summary>
	static std::string blame(std::string repoPath, std::string filePath);

	/// <summary>
	/// blame mirrors the Blame functionality of the Git system; it requires the path
	/// to a cloned Git Repo and the paths to the file to blame, and returns a string 
	/// containing the author data per line.
	/// </summary>
	static std::string blame(std::string repoPath, std::vector<std::string> filePath);

	/// <summary>
	/// blameToFile mirrors the Blame functionality of the Git system; it requires the
	/// path to a cloned Git Repo and the path to a file to blame, together with a path 
	/// to write the blame data to.
	/// </summary>
	static void blameToFile(std::string repoPath, std::string filePath, std::string outputFile);

	/// <summary>
	/// blameToFile mirrors the Blame functionality of the Git system; it requires the
	/// path to a cloned Git Repo and the paths to the files to blame, together with the paths
	/// to write the blame data to.
	/// </summary>
	static void blameToFile(std::string repoPath, std::vector<std::string> filePath, std::vector<std::string> outputFile);
};

