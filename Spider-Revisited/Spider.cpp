/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Spider.h"
#include <chrono>
#include <iostream>

// TODO: Add combining of exit codes.
AuthorData Spider::download(std::string url, std::string filePath)
{
	downloadSource(url, filePath);
	downloadMetaData(url, filePath);
	//auto start = std::chrono::high_resolution_clock::now();
    AuthorData output = downloadAuthor(url, filePath);
	//auto finish = std::chrono::high_resolution_clock::now();
	//auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
	//std::cout << microseconds.count() << "us\n";
	return output;
}
