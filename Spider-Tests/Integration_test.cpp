/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "pch.h"

#include "RunSpider.h"

#include <string>
#include <fstream>
#include <filesystem>

TEST(IntegrationTest, BasicParse)
{
	auto ret = RunSpider::runSpider("https://github.com/SoftwareProj2021/TestRepo", "Downloads", 1, "HEAD", "");

	// File names of files in project.
	std::string files[] = {"File0.cpp", "File1.c", "LF line breaks.c", "Headers/File0.h", "Headers/File1.h", "Headers/Folder0/Folder1/file3.cpp"};
	std::string filesB[] = {"File0.cpp", "File1.c", "LF line breaks.c", "Headers\\File0.h", "Headers\\File1.h", "Headers\\Folder0\\Folder1\\file3.cpp"};

	// Check if all files are found.
	ASSERT_EQ(ret.size(), 6);

	// Check existance and some author data of each file.
	for (int i = 0; i < 6; i++)
	{
		auto file = ret.find(files[i]);
		std::vector<CodeBlock> commitData;
		
		// Check if file exists with frontslashes.
		if (file == ret.end())
		{
			// Check if file exists with backslashes.
			file = ret.find(filesB[i]);

			if (file == ret.end())
			{
				// Force error if file not found with either front or back slashes.
				ASSERT_TRUE(false);
			}

			commitData = ret[filesB[i]];
		}
		else
		{
			commitData = ret[files[i]];
		}


		// Check some author data.
		EXPECT_EQ(commitData[0].commit->author, "SoftwareProj2021");
		EXPECT_EQ(commitData[0].commit->committer, "GitHub");
		EXPECT_EQ(commitData[0].commit->committerMail, "<noreply@github.com>");
	}
}
TEST(IntegrationTest, LineBreaksConversion)
{
	auto ret = RunSpider::runSpider("https://github.com/SoftwareProj2021/TestRepo", "Downloads", 1, "HEAD", "");
	std::string path = "Downloads\\\\LF line breaks.c";
	std::ifstream fin(path, std::ios::binary);
	std::string content((std::istreambuf_iterator<char>(fin)),
		(std::istreambuf_iterator<char>()));
	std::string correctLineBreak = "\r\n";
	std::string incorrectLineBreak = "\n";

	if (content.find(correctLineBreak) != std::string::npos)
	{
		std::string::size_type i = content.find(correctLineBreak);
		while (i != std::string::npos) //From https://stackoverflow.com/questions/32435003/how-to-remove-all-substrings-from-a-string/32435148.
		{
			content.erase(i, correctLineBreak.length());
			i = content.find(correctLineBreak, i);
		}
		if (content.find(incorrectLineBreak) == std::string::npos)
		{
			ASSERT_TRUE(true);
		}
		else
		{
			EXPECT_TRUE(false);
		}
	}
	else
	{
		EXPECT_TRUE(false);
	}

	

}