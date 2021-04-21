/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "Git.h"
#include "ExecuteCommand.h"

int Git::clone(std::string url, std::string filePath)
{
	std::string command = "git clone " + url + " " + filePath + " --no-checkout --branch master";
	command.append(" && cd " + filePath + " && git sparse-checkout set ");
	command.append("*.c *.cpp *.h *.cs *.cc *.hpp *.java"); // TODO: read these from file;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	// Sadly does not work, although it should
	//command.append(" ![Cc][Oo][Nn].* ![Pp][Rr][Nn].* ![Aa][Uu][Xx].* ![Nn][Uu][Ll].* ![Cc][Oo][Mm][123456789].* ![Ll][Pp][Tt][123456789].*");
#endif
	command.append(" && git checkout master");
	ExecuteCommand::exec(command.c_str());
	return 0;
}

std::string Git::blame(std::string filePath)
{
	return std::string();
}
