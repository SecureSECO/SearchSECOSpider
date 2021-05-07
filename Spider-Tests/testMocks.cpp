/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include <map>
#include <string>
#include <vector>
#include "pch.h"
#include "testMocks.h"

// Splits a filepath into directories.
std::vector<std::string> splitFilepath(std::string filePath)
{
	// Init vars.
	std::vector<std::string> res;
	size_t prev = 0;
	size_t found = filePath.find_first_of("/\\");

	// Add folder names to vector.
	while (found != std::string::npos)
	{
		res.push_back(filePath.substr(prev, found - prev));
		prev = found + 1;
		found = filePath.find_first_of("/\\", prev);
	}

	// Add file and return.
	res.push_back(filePath.substr(prev));
	return res;
}

std::string FilesystemMock::readFile(std::string filePath)
{
	// Get folder names.
	auto path = splitFilepath(filePath);
	Node *node = mainNode;

	// Recursively loop through nodes.
	for (int i = 0; i < path.size(); i++)
	{
		auto it = node->children.find(path[i]);
		if (it == node->children.end())
		{
			throw "File to read from doesn't exist.";
		}

		node = &(*it).second;
	}

	// Return contents of node.
	if (node->isFolder)
	{
		throw "Tried to read from folder.";
	}
	return node->contents;
}

FilesystemMock::~FilesystemMock()
{
    delete mainNode;
}
