/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "GitSpider.h"
#include "Git.h"

int GitSpider::downloadSource(std::string url, std::string filePath)
{
	Git::clone(url, filePath);
	return 0;
}

int GitSpider::downloadMetaData(std::string url, std::string filePath)
{
	return 0;
}

int GitSpider::downloadAuthor(std::string url, std::string filePath)
{
	return 0;
}
