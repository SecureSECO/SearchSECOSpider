/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// Spider includes.
#include "Spider.h"

#define EXTS ".c .cpp .h .cs .cc .hpp .java .py .js"


class RunSpider
{
public:

	/// <summary>
	/// Sets up specific spider, based on host website of the project and max number of threads.
	/// </summary>
	/// <param name="url"> Link to repository to download. </param>
	/// <param name="filePath"> Local path where to store the repository. </param>
	/// <param name="threads"> Amount of threads the spider can use. </param>
	/// <returns> Pointer to Spider. </returns>
	static Spider *setupSpider(std::string const &url, int threads);

	/// <summary>
	/// Downloads a repository from a given source and stores it
	/// locally at the location defined by filePath.
	/// </summary>
	/// <param name="spider"> Specific spider to use. </param>
	/// <param name="url"> Url to source to download. </param>
	/// <param name="filePath"> Local path where to store the source.</param>
	/// <param name="branch"> Branch of the source to download. </param>
	static void downloadRepo(Spider *spider, std::string const &url, std::string const &filePath,
						std::string const &branch = "");

	/// <summary>
	/// Updates project from one version to another, keeping track of unchanged files.
	/// Versions should be in chronological order. Deletes unchanged files from local project.
	/// </summary>
	/// <param name="spider"> Specific spider to use. </param>
	/// <param name="filePath"> Local path where project is stored. </param>
	/// <param name="prevTag"> Name of current version. </param>
	/// <param name="newTag"> Name of version to update to. </param>
	/// <param name="prevUnchangedFiles"> Name of previous unchanged files, which were
	/// deleted from the local project. </param>
	/// <returns> Unchanged files between versions. </returns>
	static std::vector<std::string> updateVersion(Spider *spider, std::string const &filePath,
												  std::string const &prevTag, std::string const &newTag,
												  std::vector<std::string> prevUnchangedFiles);

	/// <summary>
	/// Switches local project to different version.
	/// </summary>
	/// <param name="spider"> Specific spider to use. </param>
	/// <param name="tag"> Name of the version to update to. </param>
	/// <param name="filePath"> Local path where project is stored. </param>
	static void switchVersion(Spider *spider, std::string const &tag, std::string const &filePath);

	/// <summary>
	/// Trims the local files to only keep the specified ones.
	/// </summary>
	/// <param name="spider"> Specific spider to use. </param>
	/// <param name="lines"> The files to keep. </param>
	/// <param name="filePath"> Local path where project is stored. </param>
	static void trimFiles(Spider *spider, std::map<std::string, std::vector<int>> const lines, std::string const &filePath);

	/// <summary>
	/// Extracts author data from locally stored project.
	/// </summary>
	/// <param name="spider"> Specific spider to use. </param>
	/// <param name="filePath"> Local path where project is stored. </param>
	/// <returns> AuthorData object containing extracted author data. </returns>
	static AuthorData getAuthors(Spider *spider, std::string const &filePath);

	/// <summary>
	/// Gets tags from downloaded repository.
	/// </summary>
	/// <param name="filePath"> Location of repository. </param>
	/// <returns> List of tags and their UNIX timestamp. Sorted from newest to oldest. </returns>
	static std::vector<std::tuple<std::string, long long, std::string>> getTags(std::string const &filePath);

	/// <summary>
	/// Returns commit hash of a tag or HEAD.
	/// </summary>
	/// <param name="tag">Tag or HEAD.</param>
	/// <returns>Commit hash</returns>
	static std::string getCommitHash(std::string const& tag, std::string const& filePath);

	/// <summary>
	/// Checks if an URL is valid and returns the appropriate spider subclass to download the url.
	/// </summary>
	/// <param name="url"> URL to check. </param>
	/// <returns> Spider object capable of downloading the url. </returns>
	static Spider *getSpider(std::string const &url);
};
