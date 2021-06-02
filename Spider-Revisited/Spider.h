/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include <string>
#include "CodeBlock.h"

class Spider
{
public:
	/// <summary>
	/// Downloads an entire project, including project and author meta-
	/// data. It is supplied with an URL to the project in question and the name
	/// of the directory into which to download all the data. Returns a data
	/// structure containing the author data.
	/// </summary>
	/// <param name="url"> Source to download. </param>
	/// <param name="filePath"> Where to store the source locally. </param>
	/// <param name="branch"> Which branch of the source to download. </param>
	/// <returns></returns>
	virtual AuthorData download(std::string const &url, std::string const &filePath, std::string const &branch);

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

protected:
	// Amount of threads the spider can use.
	int threadsCount;

	// Sets the extensions the spider should download.
	std::string parsableExts;

private:
	/// <summary>
	/// Downloads a repository of a given source and stores it
	/// locally at the location defined by filePath.
	/// </summary>
	/// <param name="url"> Url to source to download. </param>
	/// <param name="filePath"> Local path where to store the source.</param>
	/// <param name="branch"> Branch of the source to download. </param>
	/// <returns> Error code. </returns>
	virtual int downloadSource(std::string const &url, std::string const &filePath, std::string const &branch) = 0;

	/// <summary>
	/// downloadAuthor gets the author data for each file using git blame.
	/// Required is that this is the same path as is supplied
	/// to downloadSource. downloadSource needs to be called before this 
	/// method. Returns a data structure containing the author data.
	/// </summary>
	/// <param name="filePath"> Local location of the repository. </param>
	/// <returns> AuthorData of the files in the repository. </returns>
	virtual AuthorData downloadAuthor(std::string const &filePath) = 0;
};

