/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Spider includes.
#include "Filesystem.h"

// External includes.
#include <fstream>


std::string FilesystemImp::readFile(std::string const &filePath)
{
	std::ifstream file;
	file.exceptions(std::ifstream::badbit);
	file.open(filePath);
	std::string res((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	return res;
}

std::queue<std::filesystem::path> FilesystemImp::getFilepaths(std::string const &repoPath,
													std::function<bool(std::filesystem::directory_entry)> predicate)
{
	std::queue<std::filesystem::path> files;
	for (const auto &path : std::filesystem::recursive_directory_iterator(repoPath))
	{
		if (predicate(path))
		{
			files.push(path);
		}
	}
	return files;
}

bool FilesystemImp::isRegularFile(std::string const &path)
{
	return std::filesystem::is_regular_file(path);
}

void FilesystemImp::remove(std::string const &path)
{
	std::filesystem::remove(path);
}

FilesystemImp::~FilesystemImp()
{
}
