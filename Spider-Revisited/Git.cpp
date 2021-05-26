/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <map>
#include <memory>
#include <sstream>
#include <thread>
#include <vector>

#include "ExecuteCommand.h"
#include "Filesystem.h"
#include "Git.h"
#include "Logger.h"
#include "Error.h"

std::string Git::getCloneCommand(std::string const &url, std::string const &filePath, std::string const &branch)
{
	std::string command = "git clone " + url + " \"" + filePath + "\" --no-checkout --branch " + branch;
	command.append(" && cd \"" + filePath + "\" && git sparse-checkout set ");
	command.append(GetFileExtensions("extensions"));

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	// Sadly does not work, although it should.
	// command.append(" ![Cc][Oo][Nn].* ![Pp][Rr][Nn].* ![Aa][Uu][Xx].* ![Nn][Uu][Ll].* ![Cc][Oo][Mm][123456789].*
	// ![Ll][Pp][Tt][123456789].*");
#endif

	command.append(" && git checkout " + branch);
	return command;
}

int Git::clone(std::string const &url, std::string const &filePath, std::string const &branch)
{
	std::string command = getCloneCommand(url, filePath, branch);
	int tries = RECONNECT_TRIES;
	int delay = RECONNECT_DELAY;

	std::string str = ExecuteCommand::execOut(command.c_str());
	
	// Retry loop incase cloning fails to respond.
	while (str == "")
	{
		// Exit with error code if no more tries left.
		if (tries < 0)
		{
			Logger::logFatal(Error::getErrorMessage(ErrorType::GitCloneError), __FILE__, __LINE__, (int)ErrorType::GitCloneError);
			throw 1;
		}

		// Try again after delay.
		std::cout << "Download failed, trying again in " << delay << " seconds... " << tries << " tries left." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(delay));
		delay *= 2;
		tries--;
		
		str = ExecuteCommand::execOut(command.c_str());
	}

	return 0;
}

std::string Git::getBlameToFileCommand(std::string const &repoPath, std::vector<std::string> const &filePath,
									   std::vector<std::string> const &outputFile)
{
	// Git blame can only be used from the Git folder itself, so go there...
	std::string command = "cd \"" + repoPath + "\"";
	// ...before blaming.
	for (int i = 0; i < filePath.size(); i++)
	{
		command.append(" && git blame -p \"" + filePath[i] + "\" >> \"" + outputFile[i] + "\"");
	}
	return command;
}

void Git::blameToFile(std::string const &repoPath, std::vector<std::string> const &filePath, std::vector<std::string> const &outputFile)
{
	std::string command = getBlameToFileCommand(repoPath, filePath, outputFile);
	ExecuteCommand::exec(command.c_str());
}

std::vector<CodeBlock> Git::getBlameData(std::string const &filePath)
{
	std::string blameData = Filesystem::readFile(filePath);
	if (blameData != "")
	{
		return parseBlame(blameData);
	}
	return std::vector<CodeBlock>();
}

std::string Git::GetFileExtensions(std::string const &extensionsFile)
{
	// Read extentions from file. Error catching based on https://stackoverflow.com/questions/9670396/exception-handling-and-opening-a-file.
	std::string contents;
	try
	{
		contents = Filesystem::readFile(extensionsFile);
	}
	catch (const std::ifstream::failure& e)
	{
		Logger::logWarn(Error::getErrorMessage(ErrorType::FileExtensionsNotFound), __FILE__, __LINE__);
	}
	std::vector<std::string> fileExts;

	// Based on https://stackoverflow.com/questions/12514510/iterate-through-lines-in-a-string-c.
	std::istringstream iss(contents);
	for (std::string line; std::getline(iss, line);)
	{
		if (line.length() > 0)
		{
			fileExts.push_back(line);
		}
	}

	// Format file extensions in a string.
	std::string output;
	for (int i = 0; i < fileExts.size(); i++)
	{
		output += "*" + fileExts[i];
		if (i != fileExts.size() - 1)
		{
			output += " ";
		}
	}

	return output;
}

// Separates a string on given character.
std::vector<std::string> split(std::string const &str, char c)
{
	// Prepare input
	std::stringstream ss(str);
	std::string to;
	std::vector<std::string> lines;

	while (std::getline(ss, to, c))
	{
		lines.push_back(to);
	}
	return lines;
}

// Combines all string in a vector separated by a space, first element is ignored.
std::string combine(std::vector<std::string> const &string)
{
	std::string res;
	for (int i = 1; i < string.size(); i++)
	{
		res += string[i];
		if (i != string.size() - 1)
		{
			res += " ";
		}
	}
	return res;
}

/*
 * Based on 'GitBlameParserJS' by Matt Pardee.
 * https://github.com/mattpardee/GitBlameParserJS
 */
std::vector<CodeBlock> Git::parseBlame(std::string const &blameData)
{
	// Split file into lines.
	std::vector<std::string> lines = split(blameData, '\n');

	// Set up data.
	std::map<std::string, std::shared_ptr<CommitData>> commitdata;
	std::vector<CodeBlock> codedata;
	int codeblocks = -1;
	bool settingCommitData = false;
	std::string currentCommitHash = "";
	std::string previousCommitHash = "";

	// Loop over all lines.
	for (int i = 0; i < lines.size(); i++)
	{
		// Commits end with code line.
		// Code is prefixed with a tab.
		if (lines[i][0] == '\t')
		{
			settingCommitData = false;
			previousCommitHash = currentCommitHash;
			continue;
		}

		// Split in line into parts.
		auto arrLine = split(lines[i], ' ');

		// Store data into commit data.
		if (settingCommitData)
		{
			parseCommitLine(currentCommitHash, commitdata, arrLine);
			continue;
		}

		// Verify that data is in a valid format.
		if (arrLine.size() < 3)
		{
			Logger::logFatal(Error::getErrorMessage(ErrorType::BlameIncorrectFormat), __FILE__, __LINE__, (int)ErrorType::BlameIncorrectFormat);
			throw 1;
		}

		// Store new commit.
		currentCommitHash = arrLine[0];
		// Check if commit is different from previous line.
		if (currentCommitHash != previousCommitHash)
		{
			// Create new commitdata if commithash can't be found.
			if (commitdata.find(currentCommitHash) == commitdata.end())
			{
				settingCommitData = true;
				commitdata[currentCommitHash] = std::make_shared<CommitData>();
			}

			// Add new codeblock.
			codeblocks++;
			codedata.push_back({
				std::stoi(arrLine[2]),
				1,
				commitdata[currentCommitHash],
			});
		}
		// Increase length of codeblock if commit is the same as previous.
		else
		{
			codedata[codeblocks].numLines++;
		}
	}
	return codedata;
}

/*
 * Based on 'GitBlameParserJS' by Matt Pardee.
 * https://github.com/mattpardee/GitBlameParserJS
 */
void Git::parseCommitLine(std::string const &commit, std::map<std::string, std::shared_ptr<CommitData>> &commitData,
						  std::vector<std::string> const &line)
{

	// Verify that data is in a valid format.
	if (line.size() < 2)
	{
		if (line[0] == "boundary")
		{
			return;
		}
		Logger::logFatal(Error::getErrorMessage(ErrorType::BlameIncorrectFormat), __FILE__, __LINE__, (int)ErrorType::BlameIncorrectFormat);
		throw 1;
	}

	// Check what type of data is found and store it.
	if (line[0] == "author")
	{
		commitData[commit]->author = combine(line);
	}
	else if (line[0] == "author-mail")
	{
		commitData[commit]->authorMail = line[1];
	}
	else if (line[0] == "author-time")
	{
		commitData[commit]->authorTime = line[1];
	}
	else if (line[0] == "author-tz")
	{
		commitData[commit]->authorTz = line[1];
	}
	else if (line[0] == "committer")
	{
		commitData[commit]->committer = combine(line);
	}
	else if (line[0] == "committer-mail")
	{
		commitData[commit]->committerMail = line[1];
	}
	else if (line[0] == "committer-time")
	{
		commitData[commit]->committerTime = line[1];
	}
	else if (line[0] == "committer-tz")
	{
		commitData[commit]->committerTz = line[1];
	}
	else if (line[0] == "summary")
	{
		commitData[commit]->summary = combine(line);
	}
	else if (line[0] == "filename")
	{
		commitData[commit]->fileName = line[1];
	}
	else if (line[0] == "previous")
	{
		commitData[commit]->previousHash = combine(line);
	}
}