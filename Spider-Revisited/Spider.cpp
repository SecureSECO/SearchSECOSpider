/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Spider.h"
#include <iostream>
#include <map>

// TODO: Add combining of exit codes.
AuthorData Spider::download(std::string url, std::string filePath)
{
	int result = downloadSource(url, filePath);
	// Check if downloading source failed.
	if (result != 0)
	{
		std::cout << "Failed to clone '" << url << "'.";
		return AuthorData();
	}

	downloadMetaData(url, filePath);

	AuthorData output = downloadAuthor(url, filePath);

	return output;
}
