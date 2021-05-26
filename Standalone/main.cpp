/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "RunSpider.h"

int main()
{
	int code = 0;
	RunSpider::runSpider("https://github.com/zavg/linux-0.01", "Downloads", 2, code);
	return code;
}