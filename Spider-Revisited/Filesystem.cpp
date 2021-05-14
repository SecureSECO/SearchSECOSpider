/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include <fstream>
#include <streambuf>
#include <string>
#include "Filesystem.h"

std::string FilesystemImp::readFile(std::string filePath)
{
	std::ifstream file;
	file.exceptions(std::ifstream::badbit);
	file.open(filePath);
	std::string res((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	return res;
}

FilesystemImp::~FilesystemImp()
{
}
