#ifndef SCRAPER_H
#define SCRAPER_H
#include <unordered_map>
#include <string>

#include <curl/curl.h>
#include <libxml/HTMLparser.h>


class Site
{
public:
    Site(){}
//    Site(std::string & tag);
    Site(const std::string &name, const std::string &&tag);



private:

    bool initCurl(const char *url);
    static int writer(char *data, size_t size, size_t nmemb, std::string *writerData);

    std::string tag;
    std::string data;

    CURL *conn;
    CURLcode code;
    std::string htmlData;
    char errorBuffer[CURL_ERROR_SIZE];



};

class Scraper
{
public:
    Scraper();

    void startScraping(void);
    std::string getHeadline(std::string &webSite);

private:
    void parseHtml(const std::string &html,
                          std::string &title);


    void operator()(const std::string& web);

//    static Scraper instance;
    std::unordered_map<std::string, Site> webSites;


};
#endif // SCRAPER_H
