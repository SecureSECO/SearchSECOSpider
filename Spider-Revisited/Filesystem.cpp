/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include <filesystem>
#include <fstream>
#include <functional>
#include <queue>
#include <streambuf>
#include <string>
#include "Filesystem.h"

std::string FilesystemImp::readFile(std::string const &filePath)
{
	std::ifstream file;
	file.exceptions(std::ifstream::badbit);
	file.open(filePath);
	std::string res((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	return res;
}

std::queue<std::string> FilesystemImp::getFilepaths(std::string const &repoPath,
													std::function<bool(std::filesystem::directory_entry)> predicate)
{
	std::queue<std::string> files;
	for (const auto &path : std::filesystem::recursive_directory_iterator(repoPath))
	{
		if (predicate(path))
		{
			std::string s = (path.path()).string();
			files.push(s);
		}
	}
	return files;
}

bool FilesystemImp::isRegularFile(std::string const &path)
{
	return std::filesystem::is_regular_file(path);
}

FilesystemImp::~FilesystemImp()
{
}
