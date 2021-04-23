/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/
#include <fstream>
#include <memory>
#include <map>
#include <sstream>
#include <vector>

#include "Git.h"
#include "ExecuteCommand.h"

int Git::clone(std::string url, std::string filePath)
{
	std::string command = "git clone " + url + " " + filePath + " --no-checkout --branch master";
	command.append(" && cd " + filePath + " && git sparse-checkout set ");
	command.append("*.c *.cpp *.h *.cs *.cc *.hpp *.java"); // TODO: read these from file;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	// Sadly does not work, although it should.
	//command.append(" ![Cc][Oo][Nn].* ![Pp][Rr][Nn].* ![Aa][Uu][Xx].* ![Nn][Uu][Ll].* ![Cc][Oo][Mm][123456789].* ![Ll][Pp][Tt][123456789].*");
#endif
	command.append(" && git checkout master");
	ExecuteCommand::exec(command.c_str());
	return 0;
}

std::string Git::blame(std::string repoPath, std::string filePath)
{
	// Git blame can only be used from the Git folder itself, so go there...
	std::string command = "cd " + repoPath;
	// ...before blaming.
	command.append(" && git blame -p " + filePath);
	return ExecuteCommand::execOut(command.c_str());
}

void Git::blameToFile(std::string repoPath, std::string filePath, std::string outputFile)
{
	// Git blame can only be used from the Git folder itself, so go there...
	std::string command = "cd " + repoPath;
	// ...before blaming.
	command.append(" && git blame -p " + filePath + " >> " + outputFile);
	ExecuteCommand::exec(command.c_str());
}

// Separates a string on given character.
std::vector<std::string> split(std::string str, char c)
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
std::string combine(std::vector<std::string> &string)
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
 * Based on 'GitBlameParserJS' by Matt Pardee
 * https://github.com/mattpardee/GitBlameParserJS
 */
void Git::parseCommitLine(std::string &commit, std::map<std::string, std::shared_ptr<CommitData>> &commitData,
                          std::vector<std::string> &line)
{
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

/*
 * Based on 'GitBlameParserJS' by Matt Pardee
 * https://github.com/mattpardee/GitBlameParserJS
 */
std::vector<CodeBlock> Git::parseBlame(std::string arg)
{
    // Split file into lines.
    std::vector<std::string> lines = split(arg, '\n');

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
            currentCommitHash = "";
        }
        else
        {
            auto arrLine = split(lines[i], ' ');

            // Store data into commit data.
            if (settingCommitData)
            {
                parseCommitLine(currentCommitHash, commitdata, arrLine);
            }
            // Store new commit.
            else
            {
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
                // Increase length of codeblock if commit is the same.
                else
                {
                    codedata[codeblocks].numLines++;
                }
            }
        }
    }
    return codedata;
}