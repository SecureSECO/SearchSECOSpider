/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include "Spider.h"

class GitSpider : public Spider
{
private:
	/// <summary>
	/// Implements the abstract downloadSource method from the Spider class.
	/// </summary>
	int downloadSource(std::string url, std::string filePath) override;

	/// <summary>
	/// Implements the abstract downloadMetaData method from the Spider class.
	/// </summary>
	int downloadMetaData(std::string url, std::string filePath) override;

	/// <summary>
	/// Implements the abstract downloadAuthor method from the Spider class.
	/// </summary>
	int downloadAuthor(std::string url, std::string filePath) override;
};

