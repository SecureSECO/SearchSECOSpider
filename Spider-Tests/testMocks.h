/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include <map>
#include <string>

#include "Filesystem.h"

// Represents a Node in a filesystem.
class Node
{
public:
	std::string name;
	std::string contents;
	bool isFolder;
	std::map<std::string, Node> children;

	// Blank constructor.
	Node()
	{
		isFolder = true;
	};
	
	// Folder constructor.
	Node(std::string name)
	{
		isFolder = true;
		this->name = name;
	}

	// File constructor.
	Node(std::string name, std::string contents)
	{
		isFolder = false;
		this->name = name;
		this->contents = contents;
	}
};

// Mock file system for tests.
class FilesystemMock : public FilesystemImp
{
public:
	Node* mainNode;

	FilesystemMock() { mainNode = new Node("main"); }
	/// <summary>
	/// Reads from node-based filesystem.
	/// </summary>
	std::string readFile(std::string filePath) override;
	~FilesystemMock();
};