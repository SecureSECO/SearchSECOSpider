/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "RunSpider.h"

int main()
{
	Spider *s = RunSpider::setupSpider("https://github.com/nlohmann/json", 2);
	RunSpider::downloadRepo(s, "https://github.com/nlohmann/json", "Downloads");
	AuthorData a = RunSpider::getAuthors(s, "Downloads");
	delete s;
	return 0;
}