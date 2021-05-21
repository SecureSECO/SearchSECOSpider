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

#define RECONNECT_TRIES 2
#define RECONNECT_DELAY 1

class Git
{
private:
	/// <summary>
	/// Parses a line of data in a commit.
	/// </summary>
	/// <param name="commit"> Which commit the data is part of. </param>
	/// <param name="commitData"> Commitdata structure to store data in. </param>
	/// <param name="line"> The line to parse. </param>
	void parseCommitLine(std::string &commit, std::map<std::string, std::shared_ptr<CommitData>> &commitData,
								std::vector<std::string> &line);

	/// <summary>
	/// Gets the command to blame a file.
	/// </summary>
	/// <param name="repoPath">The path to the local repository.</param>
	/// <param name="filePath">A list of paths to the files.</param>
	/// <returns>A string representing the command.</returns>
	std::string getBlameCommand(std::string repoPath, std::vector<std::string> filePath);

	/// <summary>
	/// Gets the command to blame a file to a file.
	/// </summary>
	/// <param name="repoPath">The path to the local repository.</param>
	/// <param name="filePath">A list of paths to the files.</param>
	/// <param name="outputFile">A list of paths to the in which the output needs to be written.</param>
	/// <returns>A string representing the command.</returns>
	std::string getBlameToFileCommand(std::string repoPath, std::vector<std::string> filePath, std::vector<std::string> outputFile);

	/// <summary>
	/// Gets the command to clone a project.
	/// </summary>
	/// <param name="url">The url from which the project needs to be cloned.</param>
	/// <param name="filePath">The path to which the project gets cloned to.</param>
	/// <param name="branch">The branch that needs to be cloned. 
	/// Giving an empty string will return the master branch.</param>
	/// <returns>A string representing the command.</returns>
	std::string getCloneCommand(std::string url, std::string filePath, std::string branch);

public:
	/// <summary>
	/// Mirrors the full Clone functionality of the Git system; it requires
	/// a HTTPS link to the Git repository to be cloned, together with the location
	/// where the repository should be cloned into and returns an integer to indicate
	/// the result of the clone operation (success or specific failure).
	/// </summary>
	/// <param name="url"> Url to repository. </param>
	/// <param name="filePath"> Location to store repository locally. </param>
	/// <param name="branch"> Branch of repository to download. </param>
	/// <returns> Error code. </returns>
	int clone(std::string url, std::string filePath, std::string branch);

	/// <summary>
	/// Mirrors the Blame functionality of the Git system; it requires the
	/// path to a cloned Git Repo and the paths to the files to blame, together with the paths
	/// to write the blame data to.
	/// </summary>
	/// <param name="repoPath"> Path to local repository. </param>
	/// <param name="filePath"> Files to blame inside local repository. </param>
	/// <param name="outputFile"> Files to write output to. </param>
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
	/// <param name="blameData"> Blame data to parse. </param>
	/// <returns> Codeblock created from blame data. </returns>
	std::vector<CodeBlock> parseBlame(std::string blameData);

	/// <summary>
	/// Reads all file extensions from the 'extensions' file.
	/// </summary>
	/// <param name="extensionsFile"> Name of the file containing the extensions. </param>
	/// <returns>< Returns all file extensions as string in a vector. /returns>
	static std::string GetFileExtensions(std::string extensionsFile);
};
