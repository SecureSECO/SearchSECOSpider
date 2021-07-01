/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "pch.h"
#include <filesystem>
#include <map>
#include <queue>
#include <string>
#include <vector>
#include "testMocks.h"

// Splits a filepath into directories.
std::vector<std::string> splitFilepath(std::string const &filePath)
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

// Recursively loop through nodes and return result node.
Node *findNode(Node *node, std::vector<std::string> const &path)
{
	
	for (int i = 0; i < path.size(); i++)
	{
		auto it = node->children.find(path[i]);
		if (it == node->children.end())
		{
			throw "File to read from doesn't exist.";
		}

		node = &(*it).second;
	}
	return node;
}

std::string FilesystemMock::readFile(std::string const &filePath)
{
	// Get folder names.
	auto pathParts = splitFilepath(filePath);
	Node *node = findNode(mainNode , pathParts);

	// Return contents of node.
	if (node->isFolder)
	{
		throw "Tried to read from folder.";
	}
	return node->contents;
}

bool FilesystemMock::isRegularFile(std::string const &path)
{
	// Get folder names.
	auto pathParts = splitFilepath(path);
	Node *node = findNode(mainNode, pathParts);

	// Return false if file is a folder.
	if (node->isFolder)
	{
		return false;
	}
	return true;
}

// Recursively add all files in a node to a queue.
void recurseFolder(Node &node, std::queue<std::filesystem::path> &queue, std::string const &path,
				   std::function<bool(std::filesystem::directory_entry)> predicate)
{
	for (auto child : node.children)
	{
		// Recurse if folder is found.
		if (child.second.isFolder)
		{
			recurseFolder(child.second, queue, path + "\\" + child.second.name, predicate);
		}
		// Add to queue if file found.
		else
		{
			std::string pathName = path + "\\" + child.second.name;

			std::filesystem::directory_entry dirEntry = 
				std::filesystem::directory_entry(std::filesystem::path(pathName));

			// Check if path fulfills predicate.
			if (predicate(dirEntry))
			{
				queue.push(pathName);
			}
		}
	}
}

std::queue<std::filesystem::path> FilesystemMock::getFilepaths(std::string const &repoPath,
									std::function<bool(std::filesystem::directory_entry)> predicate)
{
	std::queue<std::filesystem::path> files;
	Node *node = findNode(mainNode, splitFilepath(repoPath));
	recurseFolder(*node, files, repoPath, predicate);
	return files;
}

bool FilesystemMock::exists(std::string const &path)
{
	try
	{
		// Try to find the node.
		findNode(mainNode, splitFilepath(path));
	}
	catch (...)
	{
		// File doesn't exists if findNode function throws an exception.
		return false;
	}

	return true;
}

void FilesystemMock::remove(std::string const &path)
{
	auto pathSplit = splitFilepath(path);
	Node *node = mainNode;
	for (int i = 0; i < pathSplit.size()-1; i++)
	{
		auto it = node->children.find(pathSplit[i]);
		if (it == node->children.end())
		{
			throw "File to delete doesn't exist.";
		}

		node = &(*it).second;
	}
	node->children.erase(pathSplit[pathSplit.size() - 1]);
}

FilesystemMock *FilesystemMock::setFilesystemMock()
{
	FilesystemMock *fsMock = new FilesystemMock();
	Filesystem::fs = fsMock;
	return fsMock;
}


void FilesystemMock::resetFileSystem(FilesystemMock *filesystemMock)
{
	delete filesystemMock;
	Filesystem::fs = new FilesystemImp();
}

FilesystemMock::~FilesystemMock()
{
	delete mainNode;
}
