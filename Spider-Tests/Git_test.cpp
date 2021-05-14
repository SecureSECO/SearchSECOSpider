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
#include "parseBlameTestData.h"
#include "testMocks.h"
#include <iostream>
#include <string>

ExecuteCommandObjMock* setExecuteCommand()
{
	ExecuteCommandObjMock* execMock = new ExecuteCommandObjMock();
	ExecuteCommand::executeCommandObj = execMock;
	return execMock;
}

void resetExecuteCommand(ExecuteCommandObjMock* execMock)
{
	delete execMock;
	ExecuteCommand::executeCommandObj = new ExecuteCommandObj;
}

TEST(BlameParse, BasicParse)
{
	Git git;
	auto codedata = git.parseBlame(parseBlameBasicTest);
	EXPECT_TRUE(codedata.size() == 10);
	// Check if authors assigned correctly.
	EXPECT_TRUE(codedata[0].commit->author == "A");
	EXPECT_TRUE(codedata[1].commit->author == "B");
	EXPECT_TRUE(codedata[2].commit->author == "C");
	EXPECT_TRUE(codedata[3].commit->author == "B");
	EXPECT_TRUE(codedata[4].commit->author == "A");
	EXPECT_TRUE(codedata[5].commit->author == "D D");
	EXPECT_TRUE(codedata[6].commit->author == "B");
	EXPECT_TRUE(codedata[7].commit->author == "A");
	EXPECT_TRUE(codedata[8].commit->author == "D D");
	EXPECT_TRUE(codedata[9].commit->author == "A");

	// Check if commit data isn't duplicated.
	// A
	EXPECT_TRUE(codedata[0].commit == codedata[4].commit);
	EXPECT_TRUE(codedata[4].commit == codedata[7].commit);
	EXPECT_TRUE(codedata[7].commit == codedata[9].commit);
	// B
	EXPECT_TRUE(codedata[1].commit == codedata[3].commit);
	EXPECT_TRUE(codedata[3].commit == codedata[6].commit);
	// D D
	EXPECT_TRUE(codedata[5].commit == codedata[8].commit);

	// Check if every line has been stored.
	const int testDataLines = 16;
	int count = 0;
	for (int i = 0; i < codedata.size(); i++)
	{
		count += codedata[i].numLines;
	}
	EXPECT_TRUE(count == testDataLines);
}

TEST(BlameParse, CheckCommitData)
{
	Git git;
	auto codedata = git.parseBlame(parseBlameCommitDataTest);
	EXPECT_TRUE(codedata.size() == 1);
	CommitData cd = *codedata[0].commit;
	EXPECT_TRUE(cd.author == "author A");
	EXPECT_TRUE(cd.authorMail == "author-email");
	EXPECT_TRUE(cd.authorTime == "author-time");
	EXPECT_TRUE(cd.authorTz == "author-TZ");
	EXPECT_TRUE(cd.committer == "committer A");
	EXPECT_TRUE(cd.committerMail == "committer-email");
	EXPECT_TRUE(cd.committerTime == "committer-time");
	EXPECT_TRUE(cd.committerTz == "committer-TZ");
	EXPECT_TRUE(cd.fileName == "testfile.cpp");
	EXPECT_TRUE(cd.summary == "This is a test summary");
	EXPECT_TRUE(cd.previousHash == "previous-hash");
}

TEST(BlameParse, InvalidData)
{
	Git git;
	EXPECT_ANY_THROW(git.parseBlame(parseBlameInvalidCommit));
	EXPECT_ANY_THROW(git.parseBlame(parseBlameInvalidData));
}

TEST(GitTest, ReadExtensionsFile)
{
	// Setup mock.
	FilesystemMock* mock = new FilesystemMock();
	Node ext = Node("extentions", ".c\n.cpp\n.h\n.cs\n");
	mock->mainNode->children["extentions"] = ext;
	Filesystem::fs = mock;

	// Run test.
	std::string exts = Git::GetFileExtensions("extentions");
	EXPECT_EQ(exts, "*.c *.cpp *.h *.cs");

	
	// Reset filesystem implementation.
	Filesystem::fs = new FilesystemImp();
	delete mock;
}

TEST(Blame, BasicBlame)
{
	Git git;
	ExecuteCommandObjMock* execMock = setExecuteCommand();
	git.blame("linux/torvalds", std::vector<std::string> {"local/path"});
	EXPECT_EQ(execMock->execString, "cd linux/torvalds && git blame -p local/path");
	resetExecuteCommand(execMock);
}

TEST(Blame, MultipleBlame)
{
	Git git;
	ExecuteCommandObjMock* execMock = setExecuteCommand();
	git.blame("linux/t0rvalds", std::vector<std::string> {"local/path1", "local/path2", "local2/path3"});
	EXPECT_EQ(execMock->execString, "cd linux/t0rvalds && git blame -p local/path1 && git blame -p local/path2 && git blame -p local2/path3");
	resetExecuteCommand(execMock);
}


TEST(BlameToFile, BasicBlameToFile)
{
	Git git;
	ExecuteCommandObjMock* execMock = setExecuteCommand();
	git.blameToFile("linux/torvalds", std::vector<std::string> {"local/path"}, std::vector<std::string> {"test/output/location"});
	EXPECT_EQ(execMock->execString, "cd linux/torvalds && git blame -p local/path >> test/output/location");
	resetExecuteCommand(execMock);
}

TEST(BlameToFile, MultipleBlameToFile)
{
	Git git;
	ExecuteCommandObjMock* execMock = setExecuteCommand();
	git.blameToFile("linux/torvalds2", std::vector<std::string> {"local/path", "local2/path1", "p"}, std::vector<std::string> {"test/output/location1", "output/location2", "test3"});
	EXPECT_EQ(execMock->execString, "cd linux/torvalds2 && git blame -p local/path >> test/output/location1 && git blame -p local2/path1 >> output/location2 && git blame -p p >> test3");
	resetExecuteCommand(execMock);
}

