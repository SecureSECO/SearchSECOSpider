/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "pch.h"

#include "ExecuteCommand.h"
#include "ExecuteCommandMock.h"
#include "Filesystem.h"
#include "Git.h"
#include "GitSpider.h"
#include "testMocks.h"
#include <iostream>
#include <string>

TEST(SpiderDownload, BlameDataCalls)
{
	// Create mocks.
	ExecuteCommandObjMock *execMock = ExecuteCommandObjMock::setExecuteCommand();
	FilesystemMock *fsMock = FilesystemMock::setFilesystemMock();

	// Give execMock an OK reponse to pass repo download call.
	execMock->responses.push("ok");
	// Create fake file system with downloaded repository.
	fsMock->mainNode->children["outofrepo.c"] = Node("outofrepo.c", "content");
	fsMock->mainNode->children["repo"] = Node("repo");
	fsMock->mainNode->children["repo"].children[".git"] = Node(".git");
	fsMock->mainNode->children["repo"].children[".git"].children["gitfile.c"] = Node("gitfile.c","content");
	fsMock->mainNode->children["repo"].children["file0.c"] = Node("file0.c", "content");
	fsMock->mainNode->children["repo"].children["folder"] = Node("folder");
	fsMock->mainNode->children["repo"].children["folder"].children["file1.c"] = Node("file1.c", "content");
	fsMock->mainNode->children["repo"].children["folder"].children["file2.c"] = Node("file2.c", "content");
	fsMock->mainNode->children["repo"].children["folder"].children["folder0"] = Node("folder0");
	fsMock->mainNode->children["repo"].children["folder"].children["folder0"].children["file3.c"] = Node("file3.c", "content");
	fsMock->mainNode->children["repo"].children["file4.c"] = Node("file4.c", "content");

	// Run function to test.
	GitSpider gitspider;
	gitspider.setThreads(1);
	gitspider.setParsableExts(".c");
	gitspider.download("test.url", "repo", "master", "", "HEAD");

	// Ensure that a gitblame has been called to all the files.
	std::string cmd = execMock->calls[2];
	EXPECT_NE(cmd.find("file0.c.meta"), std::string::npos);
	EXPECT_NE(cmd.find("file1.c.meta"), std::string::npos);
	EXPECT_NE(cmd.find("file2.c.meta"), std::string::npos);
	EXPECT_NE(cmd.find("file3.c.meta"), std::string::npos);
	EXPECT_NE(cmd.find("file4.c.meta"), std::string::npos);

	// Make sure that files outside of repository are ignored and that gitfiles are ignored.
	EXPECT_EQ(cmd.find("outofrepo.c.meta"),  std::string::npos);
	EXPECT_EQ(cmd.find("gitfile.c.meta"),  std::string::npos);

	// Clean up.
	ExecuteCommandObjMock::resetExecuteCommand(execMock);
	FilesystemMock::resetFileSystem(fsMock);
}