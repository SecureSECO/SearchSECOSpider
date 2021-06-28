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
	// Open file.
	std::ifstream file;
	file.exceptions(std::ifstream::badbit);
	file.open(filePath);

	// Write content of file to string.
	std::string res((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	return res;
}

std::queue<std::filesystem::path> FilesystemImp::getFilepaths(std::string const &repoPath,
												std::function<bool(std::filesystem::directory_entry)> predicate)
{
	// Loop over files.
	std::queue<std::filesystem::path> files;
	for (const auto &path : std::filesystem::recursive_directory_iterator(repoPath))
	{
		// Add file to queue if path passes the predicate.
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
