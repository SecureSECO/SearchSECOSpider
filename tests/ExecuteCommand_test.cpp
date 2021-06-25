/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "pch.h"
#include "ExecuteCommand.h"

class ExecuteCommandTestP :public ::testing::TestWithParam<std::string>
{
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