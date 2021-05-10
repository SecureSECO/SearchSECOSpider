/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include "CodeBlock.h"
#include <map>
#include <string>
#include <vector>

#define RECONNECT_TRIES 5
#define RECONNECT_DELAY 1

class Git
{
private:
	void parseCommitLine(std::string &commit, std::map<std::string, std::shared_ptr<CommitData>> &commitData,
								std::vector<std::string> &line);

	std::string getBlameCommand(std::string repoPath, std::vector<std::string> filePath);
	std::string getBlameToFileCommand(std::string repoPath, std::vector<std::string> filePath, std::vector<std::string> outputFile);
	std::string getCloneCommand(std::string url, std::string filePath);

public:
	/// <summary>
	/// Mirrors the full Clone functionality of the Git system; it requires
	/// a HTTPS link to the Git repository to be cloned, together with the location
	/// where the repository should be cloned into and returns an integer to indicate
	/// the result of the clone operation (success or specific failure).
	/// </summary>
	int clone(std::string url, std::string filePath);

	/// <summary>
	/// Mirrors the Blame functionality of the Git system; it requires the path
	/// to a cloned Git Repo and the paths to the file to blame, and returns a string 
	/// containing the author data per line.
	/// </summary>
	std::string blame(std::string repoPath, std::vector<std::string> filePath);

	/// <summary>
	/// Mirrors the Blame functionality of the Git system; it requires the
	/// path to a cloned Git Repo and the paths to the files to blame, together with the paths
	/// to write the blame data to.
	/// </summary>
	void blameToFile(std::string repoPath, std::vector<std::string> filePath, std::vector<std::string> outputFile);

	/// <summary>
	/// Parses the blame data in a file and returns the corrosponding datastructure.
	/// </summary>
	/// <param name="filePath"> File to parse. </param>
	/// <returns> Datastructure created from parsed blamedata. </returns>
	std::vector<CodeBlock> getBlameData(std::string filePath);

	/// <summary>
	/// Parses the contents of a blame file puts it into a CodeBlock data structure.
	/// </summary>
	std::vector<CodeBlock> parseBlame(std::string arg);

	/// <summary>
	/// Reads all file extensions from the 'extentions' file.
	/// </summary>
	/// <returns>< Returns all file extensions as string in a vector. /returns>
	static std::string GetFileExtensions(std::string extensionsFile);
};
