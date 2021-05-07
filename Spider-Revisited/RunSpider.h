/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include <string>
#include "CodeBlock.h"

class RunSpider
{
public:
	/// <summary>
	/// This is the entry point of the Spider functionality. It is supplied
	/// a HTTPS link to the Git repository to be spidered and the name of the directory 
	/// into which the extracted files should be downloaded. It returns an exit code.
	/// </summary>
	static AuthorData runSpider(std::string url, std::string filePath);
};

