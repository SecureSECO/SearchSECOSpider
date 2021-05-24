/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include <string>

class ExecuteCommandObj
{
public:
	/// <summary>
	/// Executes a given command.
	/// </summary>
	/// <param name="cmd"> Commandline command to run. </param>
	virtual void exec(const char* cmd);

	/// <summary>
	/// Executes a given command and returns the data sent to stdout.
	/// </summary>
	/// <param name="cmd"> Commandline command to run. </param>
	/// <returns> Response from console. </returns>
	virtual std::string execOut(const char* cmd);
};

class ExecuteCommand
{
public:
	inline static ExecuteCommandObj *executeCommandObj = new ExecuteCommandObj();
	/// <summary>
	/// Executes a given command.
	/// </summary>
	/// <param name="cmd"> Commandline command to run. </param>
	static void exec(const char *cmd)
	{
		executeCommandObj->exec(cmd);
	}

	/// <summary>
	/// Executes a given command and returns the data sent to stdout.
	/// </summary>
	/// <param name="cmd"> Commandline command to run. </param>
	/// <returns> Response from console. </returns>
	static std::string execOut(const char* cmd)
	{
		return executeCommandObj->execOut(cmd);
	}
};

