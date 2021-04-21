/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Spider.h"

// TODO: Add combining of exit codes.
int Spider::download(std::string url, std::string filePath)
{
	downloadSource(url, filePath);
	downloadMetaData(url, filePath);
	downloadAuthor(url, filePath);
	return 0;
}
