/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once

// External includes.
#include <filesystem>
#include <functional>
#include <queue>
#include <string>


class FilesystemImp
{
  public:
	FilesystemImp() {};

	/// <summary>
	/// Reads all data from a file and stores it into a string.
	/// </summary>
	/// <param name="filePath"> Path to the file that should be read. </param>
	/// <returns> Contents of the file. </returns>
	virtual std::string readFile(std::string const &filePath);

	/// <summary>
	/// Recursively returns all filepaths to files in a folder.
	/// Ignores paths that don't fulfill the predicate.
	/// </summary>
	/// <param name="repoPath"> From which folder to get the files. </param>
	/// <param name="predicate"> Files are only added to queue if they can pass the predicate. </param>
	/// <returns> Queue containing the filepaths. </returns>
	virtual std::queue<std::filesystem::path>getFilepaths(std::string const &repoPath, 
		std::function<bool(std::filesystem::directory_entry)> predicate);

	/// <summary>
	/// Checks path corresponds to an existing file or directory. 
	/// </summary>
	/// <param name="path"> Path to check. </param>
	/// <returns> Bool indicating if file exists. </returns>
	virtual bool exists(std::string const& path);

	/// <summary>
	/// Checks if file at location is a regular file.
	/// </summary>
	/// <param name="path"> Path to check. </param>
	/// <returns> True if file is a regular file, false otherwise. </returns>
	virtual bool isRegularFile(std::string const &path);

	/// <summary>
	/// Deletes a files.
	/// </summary>
	/// <param name="path"> Path to delete. </param>
	virtual void remove(std::string const &path);
	
	virtual ~FilesystemImp();
};

class Filesystem
{
public:
	inline static FilesystemImp *fs = new FilesystemImp();

	/// <summary>
	/// Reads all data from a file and stores it into a string.
	/// </summary>
	/// <param name="filePath"> Path to the file that should be read. </param>
	/// <returns> Contents of the file. </returns>
	static std::string readFile(std::string filePath)
	{
		return fs->readFile(filePath);
	}

	/// <summary>
	/// Recursively returns all filepaths to files in a folder.
	/// Ignores paths that don't fulfill the predicate.
	/// </summary>
	/// <param name="repoPath"> From which folder to get the files. </param>
	/// <param name="predicate"> Files are only added to queue if they can pass the query. </param>
	/// <returns> Queue containing the filepaths. </returns>
	static std::queue<std::filesystem::path> getFilepaths(std::string const &repoPath, 
		std::function<bool(std::filesystem::directory_entry)> predicate)
	{
		return fs->getFilepaths(repoPath, predicate);    
	}

	/// <summary>
	/// Checks path corresponds to an existing file or directory. 
	/// </summary>
	/// <param name="path"> Path to check. </param>
	/// <returns> Bool indicating if file exists. </returns>
	static bool exists(std::string const& path)
	{
		return fs->exists(path);
	}

	/// <summary>
	/// Checks if file at location is a regular file.
	/// </summary>
	/// <param name="path"> Path to check. </param>
	/// <returns> True if file is a regular file, false otherwise. </returns>
	static bool isRegularFile(std::string const &path)
	{
		return fs->isRegularFile(path);
	}

	/// <summary>
	/// Deletes a file.
	/// </summary>
	/// <param name="path"> Path to delete. </param>
	static void remove(std::string const& path)
	{
		return fs->remove(path);
	}
};