/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

// Spider includes.
#include "ErrorSpider.h"
#include "ExecuteCommand.h"
#include "Filesystem.h"
#include "Git.h"
#include "Logger.h"

// External includes.
#include <iostream>
#include <thread>
#include <sstream>


// Splits string on given character.
std::vector<std::string> splitString(std::string const &str, char c)
{
	// Prepare input.
	std::stringstream ss(str);
	std::string to;
	std::vector<std::string> lines;

	while (std::getline(ss, to, c))
	{
		lines.push_back(to);
	}
	return lines;
}

std::string Git::getCheckoutTagCommand(std::string filePath, std::string nextTag)
{
	return "cd \"" + filePath + "\" && git checkout \"" + nextTag + "\" --quiet";
}

void Git::clone(std::string const &url, std::string const &filePath, std::string const &branch,
				   std::string const &exts)
{
	tryClone(url, filePath, branch, exts);

	return;
}

void Git::tryClone(std::string const &url, std::string const &filePath, std::string const &branch,
					 std::string const &exts)
{
	std::string downloadCommand = getCloneCommand(url, filePath, branch, exts);
	// Get .git folder.
	ExecuteCommand::exec(downloadCommand.c_str());

	// If target folder doesn't exist, then the git clone failed.
	if (!Filesystem::exists(filePath))
	{
		Logger::logFatal(Error::getErrorMessage(ErrorType::GitCloneError), 
			__FILE__, __LINE__, (int)ErrorType::GitCloneError);
		throw 1;
	}

	// Get default branch.
	std::string brch = branch;
	std::string command;
	if (branch.empty())
	{
		command = "cd \"" + filePath + "\" && git branch --show-current";
		brch = ExecuteCommand::execOut(command.c_str());
		brch = brch.substr(0, brch.size() - 1);
	}

	// Get files.
	command = "cd \"" + filePath + "\" && git checkout " + brch + " --quiet";
	ExecuteCommand::exec(command.c_str());
}

void Git::changeTag(std::string const &filePath, std::string const &tag)
{
	ExecuteCommand::exec(("cd \"" + filePath + "\" && git checkout \"" + tag + "\" --quiet -f && git clean -f").c_str());
	Logger::logDebug("Switched to tag: " + tag, __FILE__, __LINE__);
}

// Gets filepaths of all files that changed from 'git diff' command.
std::vector<std::filesystem::path> getFilepaths(std::string const &changes, std::string const &filePath)
{
	auto lines = splitString(changes, '\n');
	std::vector<std::filesystem::path> result;
	for (int i = 0; i < lines.size(); i++)
	{
		// Check for added file or modification.
		if (lines[i][0] == 'A' || lines[i][0] == 'M')
		{
			std::filesystem::path path = filePath + "/" + lines[i].substr(2);
			result.push_back(path.make_preferred());
		}
	}
	return result;
}

std::vector<std::string> Git::getDifference(std::string const &tag, std::string const &nextTag,
											std::string const &filePath, std::vector<std::string> prevUnchangedFiles)
{
	// Get list of changed files.
	std::vector<std::filesystem::path> changedFiles = std::vector<std::filesystem::path>();
	if (tag != "")
	{
		std::string command = "cd \"" + filePath + "\" && git diff -l 32767 --name-status " + tag + " " + nextTag;
		std::string changed = ExecuteCommand::execOut(command.c_str());
		changedFiles = getFilepaths(changed, filePath);
	}
	ExecuteCommand::exec(this->getCheckoutTagCommand(filePath, nextTag).c_str());
	Logger::logDebug("Switched to tag: " + nextTag, __FILE__, __LINE__);

	// Get all files in repository.
	auto pred = [filePath](std::filesystem::directory_entry path) {
		std::string str = path.path().string();
		return !(str.rfind(filePath + "\\.git", 0) == 0 || str.rfind(filePath + "/.git", 0) == 0) &&
			   Filesystem::isRegularFile(str);
	};
	auto files = Filesystem::getFilepaths(filePath, pred);

	// Delete all unchanged files.
	std::vector<std::string> removedFiles;
	if (tag != "")
	{
		for (int i = 0; i < prevUnchangedFiles.size(); i++)
		{
			if (std::find(changedFiles.begin(), changedFiles.end(), prevUnchangedFiles[i]) == changedFiles.end())
			{
				removedFiles.push_back(prevUnchangedFiles[i]);
			}
		}
		while (!files.empty())
		{
			std::filesystem::path file = files.front();
			files.pop();
			if (std::find(changedFiles.begin(), changedFiles.end(), file) == changedFiles.end())
			{
				// Remove local path from filepath.
				std::string fileString = file.string().substr(filePath.length() + 1);
				std::replace(fileString.begin(), fileString.end(), '\\', '/');
				removedFiles.push_back(fileString);

				// Delete file locally.
				Filesystem::remove(file.string());
			}
		}
	}

	return removedFiles;
}

std::string Git::getCloneCommand(std::string const &url, std::string const &filePath, std::string const &branch,
								 std::string const &exts)
{
	std::string command = "git clone " + url + " \"" + filePath + "\" --no-checkout --quiet";
	command.append(" && cd \"" + filePath + "\" && git sparse-checkout set ");
	command.append(exts);
	command.append(" --no-cone "); // https://github.com/SecureSECO/SearchSECOSpider/issues/17
#ifdef __linux__
	command.append(" > /dev/null 2>&1");
#else
	command.append(" --quiet");
#endif
	command.append(" && git config core.autocrlf true");

	// Switch branch if specified.
	if (!branch.empty())
	{
		command.append(" && git checkout " + branch + " --quiet");
	}

	return command;
}

void Git::blameFiles(std::string const &repoPath, std::vector<std::string> const &filePath)
{
	// Move into repo.
	std::string command = "cd \"" + repoPath + "\"";

	// Blame files.
	for (int i = 0; i < filePath.size(); i++)
	{
		std::string fp = filePath[i].substr(repoPath.length() + 1);
		command.append(" && git blame -p \"" + fp + "\" >> \"" + fp + ".meta\"");
	}
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
	std::vector<std::string> lines = splitString(blameData, '\n');

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
		auto arrLine = splitString(lines[i], ' ');

		// Store data into commit data.
		if (settingCommitData)
		{
			parseCommitLine(currentCommitHash, commitdata, arrLine);
			continue;
		}

		// Verify that data is in a valid format.
		if (arrLine.size() < 3)
		{
			// Ignore filename information.
			if (arrLine[0] == "filename")
			{
				continue;
			}

			Logger::logWarn("Blame data has incorrect format.", __FILE__, __LINE__);
			return {};
		}

		// Ignore previous commit information.
		if (arrLine[0] == "previous")
		{
			continue;
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
		// Set author to unknown if author's name isn't given.
		if (line[0] == "author")
		{
			commitData[commit]->author = "Unknown author";
			return;
		}
		// Set committer to unknown if committer's's name isn't given.
		if (line[0] == "committer")
		{
			commitData[commit]->committer = "Unknown committer";
			return;
		}
		Logger::logWarn("Blame data has incorrect format.", __FILE__, __LINE__);
		return;
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
