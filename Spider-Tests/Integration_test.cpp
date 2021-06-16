﻿/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "pch.h"

#include "RunSpider.h"

#include <string>

TEST(IntegrationTest, BasicParse)
{
	auto ret = RunSpider::runSpider("https://github.com/SoftwareProj2021/TestRepo", "Downloads", 1, "HEAD", "");

	// File names of files in project.
	std::string files[] = {"File0.cpp", "File1.c", "LF line breaks.c", "Headers/File0.h", "Headers/File1.h", "Headers/Folder0/Folder1/file3.cpp"};
	std::string filesB[] = {"File0.cpp", "File1.c", "LF line breaks.c", "Headers\\File0.h", "Headers\\File1.h", "Headers\\Folder0\\Folder1\\file3.cpp"};

	AuthorData authordata = std::get<0>(ret);
	// Check if all files are found.
	ASSERT_EQ(authordata.size(), 6);

	// Check existance and some author data of each file.
	for (int i = 0; i < 5; i++)
	{
		auto file = authordata.find(files[i]);
		std::vector<CodeBlock> commitData;
		
		// Check if file exists with frontslashes.
		if (file == authordata.end())
		{
			// Check if file exists with backslashes.
			file = authordata.find(filesB[i]);

			if (file == authordata.end())
			{
				// Force error if file not found with either front or back slashes.
				ASSERT_TRUE(false);
			}

			commitData = authordata[filesB[i]];
		}
		else
		{
			commitData = authordata[files[i]];
		}


		// Check some author data.
		EXPECT_EQ(commitData[0].commit->author, "SoftwareProj2021");
		EXPECT_EQ(commitData[0].commit->committer, "GitHub");
		EXPECT_EQ(commitData[0].commit->committerMail, "<noreply@github.com>");
	}
}