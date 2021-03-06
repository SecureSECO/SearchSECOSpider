/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Spider includes.
#include "CodeBlock.h"

class Git
{
private:
	/// <summary>
	/// Parses a line of data in a commit.
	/// </summary>
	/// <param name="commit"> Which commit the data is part of. </param>
	/// <param name="commitData"> Commitdata structure to store data in. </param>
	/// <param name="line"> The line to parse. </param>
	void parseCommitLine(std::string const &commit, std::map<std::string, std::shared_ptr<CommitData>> &commitData,
								std::vector<std::string> const &line);

	/// <summary>
	/// Gets the command to clone a project.
	/// </summary>
	/// <param name="url"> The url from which the project needs to be cloned. </param>
	/// <param name="filePath"> The path to which the project gets cloned to. </param>
	/// <param name="branch"> The branch that needs to be cloned.
	///  Giving an empty string will return the master branch. </param>
	/// <param name="exts"> Extensions that should be downloaded. </param>
	/// <returns> A string representing the command. </returns>
	std::string getCloneCommand(std::string const &url, std::string const &filePath, std::string const &branch,
								std::string const &exts);

	/// <summary>
	/// Tries to download a project from the url.
	/// </summary>
	/// <param name="url"> The url from which the project needs to be cloned. </param>
	/// <param name="filePath"> The path to which the project gets cloned to. </param>
	/// <param name="branch"> The branch that needs to be cloned. Giving an empty string will return the master 
	/// branch. </param> 
	/// <param name="exts"> Extensions that should be downloaded. </param>
	/// <returns> Response that the console gave. </returns>
	void tryClone(std::string const &url, std::string const &filePath, std::string const &branch,
							 std::string const &exts);

	/// <summary>
	/// Returns the command string to check out the tag nextTag.
	/// </summary>
	/// <param name="filePath"> The path into which the project was cloned. </param>
	/// <param name="nextTag"> Name of the next tag. </param>
	std::string getCheckoutTagCommand(std::string filePath, std::string nextTag);

public:
	// Stores which files were unchanged during difference check.
	std::vector<std::string> unchangedFiles;

	/// <summary>
	/// Mirrors the full Clone functionality of the Git system; it requires
	/// a HTTPS link to the Git repository to be cloned, together with the location
	/// where the repository should be cloned into.
	/// </summary>
	/// <param name="url"> Url to repository. </param>
	/// <param name="filePath"> Location to store repository locally. </param>
	/// <param name="branch"> Branch of repository to download. </param>
	/// <param name="exts"> Extensions that should be downloaded. </param>
	void clone(std::string const &url, std::string const &filePath, std::string const &branch,
				std::string const &exts);

	/// <summary>
	/// Updates locally cloned repo to a given tag.
	/// </summary>
	/// <param name="filePath"> The path into which the project was cloned. </param>
	/// <param name="tag"> Name of the tag to checkout. </param>
	void changeTag(std::string const &filePath, std::string const &tag);

	/// <summary>
	/// Gets difference between two tags and deletes all files that haven't been changed.
	/// </summary>
	/// <param name="tag"> Tag that came before nextTag, used to calculate differences. </param>
	/// <param name="nextTag"> Tag to download. HEAD can be given as argument to compare with newest version. </param>
	/// <param name="filePath"> Location where the local repository is. </param>
	/// <param name="prevUnchangedFiles"> Name of previous unchanged files, which were
	/// deleted from the repo. </param>
	/// <returns> Vector of removed/unchanged files. </returns>
	std::vector<std::string> getDifference(std::string const &tag, std::string const &nextTag,
										   std::string const &filePath, std::vector<std::string> prevUnchangedFiles);

	/// <summary>
	/// Mirrors the Blame functionality of the Git system; it requires the
	/// path to a cloned Git Repo and the paths to the files to blame.
	/// </summary>
	/// <param name="repoPath"> Path to local repository. </param>
	/// <param name="filePath"> Files to blame inside local repository. </param>
	void blameFiles(std::string const &repoPath, std::vector<std::string> const &filePath);

	/// <summary>
	/// Parses the blame data in a file and returns the corrosponding datastructure.
	/// </summary>
	/// <param name="filePath"> File to parse. </param>
	/// <returns> Datastructure created from parsed blamedata. </returns>
	std::vector<CodeBlock> getBlameData(std::string const &filePath);
	
	/// <summary>
	/// Parses the contents of a blame file puts it into a CodeBlock data structure.
	/// </summary>
	/// <param name="blameData"> Blame data to parse. </param>
	/// <returns> Codeblock created from blame data. </returns>
	std::vector<CodeBlock> parseBlame(std::string const &blameData);
};
