/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "pch.h"

#include "Filesystem.h"
#include "testMocks.h"
#include <functional>
#include <map>
#include <string>
#include <vector>

TEST(Filesystemmock, retrieveFiles)
{
	FilesystemMock *fs = FilesystemMock::setFilesystemMock();

	// Set up folders/files.
	fs->mainNode->children["repo"] = Node("repo");
	fs->mainNode->children["repo"].children["fold0"] = Node("fold0");
	fs->mainNode->children["repo"].children["fold0"].children["file0"] = Node("file0", "data");
	fs->mainNode->children["repo"].children["fold0"].children["file1"] = Node("file1", "data");
	fs->mainNode->children["repo"].children["fold0"].children["fold1"] = Node("fold1");
	fs->mainNode->children["repo"].children["fold0"].children["fold1"].children["file2"] = Node("file2","data");
	fs->mainNode->children["repo"].children["fold0"].children["fold1"].children["file3"] = Node("file3", "data");
	fs->mainNode->children["repo"].children["file4"] = Node("file4", "data");
	fs->mainNode->children["repo"].children["fold2"] = Node("fold2");
	fs->mainNode->children["repo"].children["fold2"].children["file5"] = Node("file5", "data");

	std::string repoPath = "repo";
	auto pred = [repoPath](std::filesystem::directory_entry path) { return true; };

	// Run target.
	auto files = Filesystem::getFilepaths(repoPath, pred);

	// Check output.
	std::vector<std::filesystem::path> paths;
	while (!files.empty())
	{
		paths.push_back(files.front());
		files.pop();
	}

	EXPECT_EQ(paths.size(), 6);
	EXPECT_NE(std::find(paths.begin(), paths.end(), "repo\\fold0\\file0")       , paths.end());
	EXPECT_NE(std::find(paths.begin(), paths.end(), "repo\\fold0\\file1")       , paths.end());
	EXPECT_NE(std::find(paths.begin(), paths.end(), "repo\\fold0\\fold1\\file2"), paths.end());
	EXPECT_NE(std::find(paths.begin(), paths.end(), "repo\\fold0\\fold1\\file3"), paths.end());
	EXPECT_NE(std::find(paths.begin(), paths.end(), "repo\\file4")              , paths.end());
	EXPECT_NE(std::find(paths.begin(), paths.end(), "repo\\fold2\\file5")       , paths.end());

	// Test exists function.
	EXPECT_EQ(Filesystem::exists("repo\\fold0\\file1"), true);
	EXPECT_EQ(Filesystem::exists("repo\\fold0\\nonexistant"), false);

	FilesystemMock::resetFileSystem(fs);
}

TEST(Filesystemmock, predicateTest)
{
	FilesystemMock *fs = FilesystemMock::setFilesystemMock();

	// Set up folders/files.
	fs->mainNode->children["repo"] = Node("repo");
	fs->mainNode->children["repo"].children["fold0"] = Node("fold0");
	fs->mainNode->children["repo"].children["fold0"].children["file0"] = Node("file0", "data");
	fs->mainNode->children["repo"].children["fold0"].children["file1"] = Node("file1", "data");
	fs->mainNode->children["repo"].children["fold0"].children["fold1"] = Node("fold1");
	fs->mainNode->children["repo"].children["fold0"].children["fold1"].children["file2"] = Node("file2", "data");
	fs->mainNode->children["repo"].children["fold0"].children["fold1"].children["file3"] = Node("file3", "data");
	fs->mainNode->children["repo"].children["file4"] = Node("file4", "data");
	fs->mainNode->children["repo"].children["fold2"] = Node("fold2");
	fs->mainNode->children["repo"].children["fold2"].children["file5"] = Node("file5", "data");

	std::string repoPath = "repo";
	auto pred = [repoPath](std::filesystem::directory_entry path) 
	{
		std::string str = path.path().string();
		return !(str.rfind(repoPath + "\\fold0", 0) == 0) && Filesystem::isRegularFile(str);
	};

	// Run target.
	auto files = Filesystem::getFilepaths(repoPath, pred);

	// Check output.
	std::vector<std::filesystem::path> paths;
	while (!files.empty())
	{
		paths.push_back(files.front());
		files.pop();
	}

	EXPECT_EQ(paths.size(), 2);
	EXPECT_NE(std::find(paths.begin(), paths.end(), "repo\\file4"), paths.end());
	EXPECT_NE(std::find(paths.begin(), paths.end(), "repo\\fold2\\file5"), paths.end());

	FilesystemMock::resetFileSystem(fs);
}

TEST(Filesystemmock, removeTest)
{
	FilesystemMock *fs = FilesystemMock::setFilesystemMock();

	// Set up folders/files.
	fs->mainNode->children["repo"] = Node("repo");
	fs->mainNode->children["repo"].children["file0"] = Node("file0", "data");
	fs->mainNode->children["repo"].children["file1"] = Node("file1", "data");

	// Delete a file.
	Filesystem::remove("repo/file0");

	// Check if file is deleted.
	auto repoChilds = fs->mainNode->children["repo"].children;
	EXPECT_EQ(repoChilds.size(), 1);
	EXPECT_EQ(repoChilds.find("file0"), repoChilds.end());
	EXPECT_NE(repoChilds.find("file1"), repoChilds.end());

	FilesystemMock::resetFileSystem(fs);
}