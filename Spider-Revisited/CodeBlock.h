/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#pragma once
#include <memory>
#include <map>
#include <vector>

/// <summary>
/// Contains various data related to the blame of a commit.
/// </summary>
struct CommitData
{
    std::string author;
    std::string authorMail;
    std::string authorTime;
    std::string authorTz;
    std::string committer;
    std::string committerMail;
    std::string committerTime;
    std::string committerTz;
    std::string summary;
    std::string previousHash;
    std::string fileName;
};

/// <summary>
/// Stores which lines belong to which commit.
/// </summary>
struct CodeBlock
{
    int line;
    int numLines;
    std::shared_ptr<CommitData> commit;
};

typedef std::map<std::string, std::vector<CodeBlock>> AuthorData;
