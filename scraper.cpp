#include "scraper.h"
using namespace  std;

Scraper Scraper::instance;

Scraper::Scraper()
{
    webSites.insert({{"www.espn.com", ""}, {"www.foxnews.com", ""}, {"www.nbcnews.com",""}});
}

Scraper& Scraper::getInstance()
{
    return instance;
}

void Scraper::startScraping(void)
{

}


std::string Scraper::getHeadline(std::string &webSite)
{
        auto it = webSites.find(webSite);


}
