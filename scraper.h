#ifndef SCRAPER_H
#define SCRAPER_H
#include <string.h>
#include <stdlib.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <thread>

#include <curl/curl.h>
#include <libxml/HTMLparser.h>

class WebTitle
{
public:

    struct Context
    {
      Context(): addTitle(false), dataExtracted(false) { }

      std::string tag;
      char **attr;
      bool addTitle;
      bool dataExtracted;
      std::string title;
    };

    WebTitle() {}
    WebTitle( const std::string &tag, const char ** attr);
    void parseHtml( );
    Context& getContext() { return  context;}
     bool createConn(const std::string &name);

private:

    bool initCurl(const char *url);
    static int writer(char *data, size_t size, size_t nmemb, std::string *writerData);
    static void StartElement(void *voidContext,
                             const xmlChar *name,
                             const xmlChar **attributes);


    static void  EndElement(void *voidContext,
                           const xmlChar *name);
    static void  cdata(void *voidContext,
                      const xmlChar *chars,
                      int length);

    static void  Characters(void *voidContext,
                           const xmlChar *chars,
                           int length);

    static void handleCharacters(Context *context,
                                 const xmlChar *chars,
                                 int length);
    void clearContext(void) ;

    Context context;

    CURL *conn{};
    CURLcode code;
    char errorBuffer[CURL_ERROR_SIZE]{};
    std::string htmlData;

    htmlSAXHandler saxHandler =
    {
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        StartElement,
        EndElement,
        nullptr,
        Characters,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        cdata,
        nullptr,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    };
};

class Scraper
{
public:
    Scraper(std::unordered_map<std::string, WebTitle> & umap);
    ~Scraper();

    void startScraping(void);
    std::string getHeadline(std::string &webSite);

private:
    std::unordered_map<std::string, WebTitle> webSites;
//    std::vector<std::thread> threadIds;
};
#endif // SCRAPER_H
