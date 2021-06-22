/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "RunSpider.h"

int main()
{
	RunSpider::runSpider("https://github.com/official-stockfish/stockfish", "Downloads", 2, "HEAD", "");
	return 0;
}