/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include <string>

class Filesystem
{
public:
	/// <summary>
	/// Reads all data from a file and stores it into a string.
	/// </summary>
	/// <param name="filePath"> Path to the file that should be read. </param>
	/// <returns></returns>
	static std::string readFile(std::string filePath);
};

