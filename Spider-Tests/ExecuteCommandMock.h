/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include "ExecuteCommand.h"

class ExecuteCommandObjMock : public ExecuteCommandObj
{
public:
	void exec(const char* cmd)
	{
		execString = cmd;
	}
	std::string execOut(const char* cmd)
	{
		execString = cmd;
		return "";
	}
	std::string execString;
};