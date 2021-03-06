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
#include "RunSpider.h"
#include "testMocks.h"
#include <iostream>
#include <string>

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
	// A.
	EXPECT_TRUE(codedata[0].commit == codedata[4].commit);
	EXPECT_TRUE(codedata[4].commit == codedata[7].commit);
	EXPECT_TRUE(codedata[7].commit == codedata[9].commit);
	// B.
	EXPECT_TRUE(codedata[1].commit == codedata[3].commit);
	EXPECT_TRUE(codedata[3].commit == codedata[6].commit);
	// D D.
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
	EXPECT_NO_THROW(git.parseBlame(parseBlameInvalidCommit));
	EXPECT_NO_THROW(git.parseBlame(parseBlameInvalidData));
}

TEST(BlameToFile, BasicBlameToFile)
{
	Git git;
	ExecuteCommandObjMock* execMock = ExecuteCommandObjMock::setExecuteCommand();
	git.blameFiles("linux/torvalds", std::vector<std::string> {"linux/torvalds/local/path"});
	EXPECT_EQ(execMock->execString, "cd \"linux/torvalds\" && git blame -p \"local/path\" >> \"local/path.meta\"");
	ExecuteCommandObjMock::resetExecuteCommand(execMock);
}

TEST(BlameToFile, MultipleBlameToFile)
{
	Git git;
	ExecuteCommandObjMock *execMock = ExecuteCommandObjMock::setExecuteCommand();
	git.blameFiles("repo", std::vector<std::string> {"repo/local/path", "repo/local2/path1", "repo/p"});
	EXPECT_EQ(execMock->execString, 
		"cd \"repo\" && git blame -p \"local/path\" >> \"local/path.meta\" &&"
		" git blame -p \"local2/path1\" >> \"local2/path1.meta\" && git blame -p \"p\" >> \"p.meta\"");
	ExecuteCommandObjMock::resetExecuteCommand(execMock);
}

TEST(CloneTest, NoResponse)
{
	Git git;
	ExecuteCommandObjMock *execMock = ExecuteCommandObjMock::setExecuteCommand();
	EXPECT_ANY_THROW(git.clone("url", "path", "branch", "*.c"));
	ExecuteCommandObjMock::resetExecuteCommand(execMock);
}

class LinkValidationParameterizedTestFixture : public ::testing::TestWithParam<std::tuple<std::string, bool>>
{
};

// Tuples of links and a bool that indicates if the link is valid.
INSTANTIATE_TEST_CASE_P(GetSpider, LinkValidationParameterizedTestFixture, 
	::testing::Values(
			std::make_tuple("nonsensehttps://www.github.com", false), 
			std::make_tuple("github.com/repository", false), 
			std::make_tuple("https://labhub.com/repository", false), 
			std::make_tuple("https://gitlab,com/repository", false),
			std::make_tuple("https://github.com/repository", true),
			std::make_tuple("https://www.gitlab.com/repository/sub/sub", true)
		)
);

TEST_P(LinkValidationParameterizedTestFixture, LinkValidity)
{
	auto data = GetParam();
	std::string link = std::get<0>(data);

	// Valid links.
	if (std::get<1>(data))
	{
		auto ptr = RunSpider::getSpider(link);
		EXPECT_NE(nullptr, ptr);
		delete ptr;
	}
	// Invalid links.
	else
	{
		EXPECT_EQ(nullptr, RunSpider::getSpider(link));
	}
}