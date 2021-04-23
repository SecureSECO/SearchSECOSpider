/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
� Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include <string>

class Spider
{
public:
	/// <summary>
	/// download downloads an entire project, including project and author meta-
	/// data. It is supplied with an URL to the project in question and the name
	/// of the directory into which to download all the data. Returns an exit code.
	/// </summary>
	virtual int download(std::string url, std::string filePath);

private:
	/// <summary>
	/// downloadSource downloads the source files of the supplied project. It
	/// needs an URL to the project and the filePath into which to download the
	/// source files. Returns an exit code.
	/// </summary>
	virtual int downloadSource(std::string url, std::string filePath) = 0;

	/// <summary>
	/// downloadMetaData downloads the project metadata. It is supplied with an
	/// URL to the project in question and the filePath into which to download
	/// the metadata. Required is that this is the same path as is supplied to 
	/// downloadSource. downloadSource needs to be called before this method.
	/// Returns an exit code.
	/// </summary>
	virtual int downloadMetaData(std::string url, std::string filePath) = 0;

	/// <summary>
	/// downloadAuthor downloads the author data per file. It is supplied with
	/// the URL to the project in question and the filePath into which to down-
	/// load the author data. Required is that this is the same path as is sup-
	/// plied to downloadSource. downloadSource needs to be called before this 
	/// method. Returns an exit code.
	/// </summary>
	virtual int downloadAuthor(std::string url, std::string filePath) = 0;
};
