/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include <string>

class ExecuteCommand
{
public:
	/// <summary>
	/// Executes a given command.
	/// </summary>
	static void exec(const char* cmd);

	/// <summary>
	/// Executes a given command and returns the data sent to stdout.
	/// </summary>
	static std::string execOut(const char* cmd);
};

