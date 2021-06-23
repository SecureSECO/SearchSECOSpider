/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include "ExecuteCommand.h"
#include <queue>
#include <string>
#include <vector>

class ExecuteCommandObjMock : public ExecuteCommandObj
{
public:
	// Stores all calls made to command prompt.
	std::vector<std::string> calls;

	// Reponses that exec out gives.
	std::queue<std::string> responses;

	// Stores most recent command.
	std::string execString;

	/// <summary>
	/// Mock of the exec function, saves the input as a string so it can later be retrieved.
	/// </summary>
	/// <param name="cmd"> Commandline command to run. </param>
	void exec(const char* cmd)
	{
		calls.push_back(cmd);
		execString = cmd;
	}

	/// <summary>
	/// Mock of the execOut function, saves the input as a string so it can later be retrieved.
	/// </summary>
	/// <param name="cmd"> Commandline command to run. </param>
	/// <returns> Response from console. </returns>
	std::string execOut(const char* cmd)
	{
		execString = cmd;
		if (responses.empty())
		{
			return "";
		}
		
		auto value = responses.front();
		responses.pop();
		return value;
	}

	/// <summary>
	/// Creates a ExecuteCommandMock object and returns it.
	/// </summary>
	/// <returns> New ExecuteCommandMock object. </returns>
	static ExecuteCommandObjMock *setExecuteCommand()
	{
		ExecuteCommandObjMock *execMock = new ExecuteCommandObjMock();
		ExecuteCommand::executeCommandObj = execMock;
		return execMock;
	}

	/// <summary>
	/// Deactivates and deletes a ExecuteCommandMock object.
	/// </summary>
	/// <param name="execMock"> ExecuteCommandMock object to be deleted. </param>
	static void resetExecuteCommand(ExecuteCommandObjMock *execMock)
	{
		delete execMock;
		ExecuteCommand::executeCommandObj = new ExecuteCommandObj;
	}
};