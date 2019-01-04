#ifndef SCRAPER_H
#define SCRAPER_H
#include <unordered_map>
#include <string>

class Scraper
{
public:
    static Scraper& getInstance();

    void startScraping(void);
    std::string getHeadline(std::string &webSite);

private:
    Scraper();
    static Scraper instance;
    std::unordered_map<std::string, std::string> webSites;
};
#endif // SCRAPER_H
