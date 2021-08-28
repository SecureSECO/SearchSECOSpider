/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Spider includes.
#include "CodeBlock.h"

// External includes.
#include <string>


class Spider
{
public:

	/// <summary>
	/// Downloads a repository of a given source and stores it
	/// locally at the location defined by filePath.
	/// </summary>
	/// <param name="url"> Url to source to download. </param>
	/// <param name="filePath"> Local path where to store the source. </param>
	/// <param name="branch"> Branch of the source to download. </param>
	virtual void download(std::string const &url, std::string const &filePath, std::string const &branch) = 0;

	/// <summary>
	/// Updates repo from one tag to another, keeping track of unchanged files.
	/// Tags should be in chronological order. Deletes unchanged files from local project.
	/// </summary>
	/// <param name="filePath"> Local path where project is stored. </param>
	/// <param name="prevTag"> Name of current version. </param>
	/// <param name="newTag"> Name of version to update to. </param>
	/// <param name="prevUnchangedFiles"> Name of previous unchanged files, which were
	/// deleted from the local project. </param>
	/// <returns> Unchanged files between versions. </returns>
	virtual std::vector<std::string> update(std::string const &filePath, std::string const &prevTag,
											std::string const &newTag, std::vector<std::string> prevUnchangedFiles) = 0;

	/// <summary>
	/// Switches local project to different version.
	/// </summary>
	/// <param name="filePath"> The path into which the project was cloned. </param>
	/// <param name="tag"> Name of the version to update to. </param>
	virtual void switchVersion(std::string const &filePath, std::string const &tag) = 0;

	/// <summary>
	/// downloadAuthor gets the author data for each file using git blame.
	/// Required is that this is the same path as is supplied
	/// to downloadSource. download needs to be called before this
	/// method. Returns a data structure containing the author data.
	/// </summary>
	/// <param name="filePath"> Local location of the repository. </param>
	/// <returns> AuthorData of the files in the repository. </returns>
	virtual AuthorData downloadAuthor(std::string const &filePath) = 0;

	/// <summary>
	/// Sets the amount of threads the Spider can use.
	/// </summary>
	/// <param name="threads"> Amount of threads. </param>
	virtual void setThreads(int threads);
	
	/// <summary>
	/// Returns the amount of threads the Spider can use.
	/// </summary>
	/// <returns> Amount of threads. </returns>
	virtual int getThreads();

	/// <summary>
	/// Sets the extensions the spider should download.
	/// Format: .c .cpp .h .cs
	/// </summary>
	virtual void setParsableExts(std::string const &exts);

	/// <summary>
	/// Get the unchanged files that were deleted by last
	/// difference check between tags.
	/// </summary>
	/// <returns> Vector of filepaths. </returns>
	virtual std::vector<std::string> getUnchangedFiles() = 0;

protected:
	// Amount of threads the spider can use.
	int threadsCount;

	// Sets the extensions the spider should download.
	std::string parsableExts;
};
