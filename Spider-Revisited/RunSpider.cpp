/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/

#include "RunSpider.h"
#include "Spider.h"
#include "GitSpider.h"

int RunSpider::runSpider(std::string url, std::string filePath)
{
    // For now, default to the Git Spider.
    Spider *spider = new GitSpider();
    spider->download(url, filePath);
    delete spider;
    return 0;
}
