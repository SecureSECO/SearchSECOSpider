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
	/*/// <summary>
	/// This is the entry point of the Spider functionality. It is supplied
	/// a HTTPS link to the Git repository to be spidered and the name of the directory
	/// into which the extracted files should be downloaded. It returns an exit code.
	/// </summary>
	/// <param name="url"> Link to repository to download. </param>
	/// <param name="filePath"> Local path where to store the repository. </param>
	/// <param name="threads"> Amount of threads the spider can use. </param>
	/// <param name="tag"> Tag that came before nextTag, used to calculate differences. </param>
	/// <param name="nextTag"> Tag to download. Pass HEAD to download most recent version. </param>
	/// <param name="branch"> Which branchs of the repository to download. </param>
	/// <returns> Authordata which contains which lines were written by which author. </returns>
	static std::tuple<AuthorData, std::string, std::vector<std::string>> runSpider(std::string const &url,
		std::string const &filePath, int threads, std::string const &tag, std::string nextTag,
		std::string const &branch = "");*/

	static Spider *setupSpider(std::string const &url, int threads);

	static void downloadRepo(Spider *spider, std::string const &url, std::string const &filePath,
						std::string const &branch = "");

	static std::vector<std::string> updateVersion(Spider *spider, std::string const &filePath,
												  std::string const &prevTag, std::string const &newTag,
												  std::vector<std::string> prevUnchangedFiles);

	static void switchVersion(Spider *spider, std::string const &tag, std::string const &filePath);

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
