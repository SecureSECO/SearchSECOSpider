/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include <functional>
#include <map>
#include <queue>
#include <string>

#include "Filesystem.h"

// Represents a Node in a filesystem.
class Node
{
public:
	std::map<std::string, Node> children;
	std::string name;
	std::string contents;
	bool isFolder;

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
	std::string readFile(std::string const &filePath) override;

	/// <summary>
	/// Returns the paths to all files in the filesystem that fulfill the predicate.
	/// </summary>
	/// <param name="repoPath"> Folder from which to get the filepaths from. </param>
	/// <param name="predicate"> Predicate to filter files. </param>
	/// <returns> Queue that has paths to the files in no particular order. </returns>
	std::queue<std::filesystem::path> getFilepaths(std::string const &repoPath, 
									std::function<bool(std::filesystem::directory_entry)> predicate) override;

	/// <summary>
	/// Checks if file at location is a regular file.
	/// </summary>
	/// <param name="path"> Path to check. </param>
	/// <returns> True if file is a regular file, false otherwise. </returns>
	bool isRegularFile(std::string const &path) override;

	/// <summary>
	/// Deletes file at location.
	/// </summary>
	/// <param name="path"> Path to delete file at. </param>
	void remove(std::string const &path) override;

	/// <summary>
	/// Creates a FilesystemMock object and returns it.
	/// </summary>
	/// <returns> New FilesystemMock object. </returns>
	static FilesystemMock *setFilesystemMock();

	/// /// <summary>
	/// Deactivates and deletes a FilesystemMock object.
	/// </summary>
	/// <param name="filesystemMock"> FilesystemMock object to be deleted. </param>
	static void resetFileSystem(FilesystemMock *filesystemMock);

	~FilesystemMock();
};