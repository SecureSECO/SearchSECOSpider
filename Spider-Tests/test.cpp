/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "pch.h"
#include "parseBlameTestData.h"
#include "../Spider-Revisited/ExecuteCommand.h"
#include "../Spider-Revisited/Git.h"
#include <string>
#include <iostream>

class ExecuteCommandTestP :public ::testing::TestWithParam<std::string> {
};

TEST_P(ExecuteCommandTestP, EchoTest) 
{
    std::string s = GetParam();
    std::string command = "echo " + s;
    std::string echo = ExecuteCommand::execOut(command.c_str());
    EXPECT_STREQ(echo.c_str(), (s + '\n').c_str());
}

INSTANTIATE_TEST_CASE_P(
    ExecuteCommandTest,
    ExecuteCommandTestP,
    ::testing::Values("hello world", "testing123", "_test_", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));

TEST(BlameParse, BasicParse)
{
    auto codedata = Git::parseBlame(parseBlameBasicTest);
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
    auto codedata = Git::parseBlame(parseBlameCommitDataTest);
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
}