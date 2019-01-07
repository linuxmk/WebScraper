#include <iostream>
#include "scraper.h"
#include <string.h>
#include <stdlib.h>
using namespace  std;

//Scraper Scraper::instance;

//Scraper& Scraper::getInstance()
//{
//    return instance;
//}


#ifdef _MSC_VER
#define COMPARE(a, b) (!_stricmp((a), (b)))
#else
#define COMPARE(a, b) (!strcasecmp((a), (b)))
#endif


static void StartElement(void *voidContext,
                         const xmlChar *name,
                         const xmlChar **attributes)
{
//  Context *context = (Context *)voidContext;


  if(COMPARE((char *)name, "div") && COMPARE((char *)*attributes, "class") && COMPARE((char *)*(attributes+1), "cluster") ) {
    fprintf(stderr, "tag = <%s>\n", (char *) name);
//    context->title = "";
//    context->addTitle = true;
  }
  (void) attributes;
}

//
//  libxml end element callback function
//

static void EndElement(void *voidContext,
                       const xmlChar *name)
{
//  Context *context = (Context *)voidContext;

  if(COMPARE((char *)name, "div") )
  {
      fprintf(stderr, "tag = </%s>\n", (char *) name);
//      context->addTitle = false;
  }
}

//
//  Text handling helper function
//

//static void handleCharacters(Context *context,
//                             const xmlChar *chars,
//                             int length)
//{
//  if(context->addTitle)
//    context->title.append((char *)chars, length);
//}

//
//  libxml PCDATA callback function
//

static void Characters(void *voidContext,
                       const xmlChar *chars,
                       int length)
{
//  Context *context = (Context *)voidContext;

//  handleCharacters(context, chars, length);
}

//
//  libxml CDATA callback function
//

static void cdata(void *voidContext,
                  const xmlChar *chars,
                  int length)
{
//  Context *context = (Context *)voidContext;

//  handleCharacters(context, chars, length);
}



static htmlSAXHandler saxHandler =
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
    nullptr
};

void Scraper::parseHtml(const std::string &html,
                      std::string &title)
{
  htmlParserCtxtPtr ctxt;
//  Context context;

  ctxt = htmlCreatePushParserCtxt(&saxHandler, NULL, "", 0, "",
                                  XML_CHAR_ENCODING_NONE);

  htmlParseChunk(ctxt, html.c_str(), html.size(), 0);
  htmlParseChunk(ctxt, "", 0, 1);

  htmlFreeParserCtxt(ctxt);

//  title = context.title;
}

Site::Site(const std::string &name, const std::string &&tag)
{
    this->tag = tag;

    if(!initCurl(name.c_str()))
        throw std::out_of_range("Can not get " + name);

    code = curl_easy_perform(conn);
    curl_easy_cleanup(conn);

    if(code != CURLE_OK) {
      throw std::out_of_range("Can not perform curl_easy_perform " );
    }
}


Scraper::Scraper()
{
    try {

        webSites.insert({
                         {"https://time.mk", Site( std::string("https://time.mk"), std::string("div") )},
                         {"www.espn.com", Site( std::string("www.espn.com"), std::string("div") )},
    //                     {"www.foxnews.com", Site(std::string("div"))},
    //                     {"www.nbcnews.com", Site(std::string("div"))}
                        }
        );

    } catch (std::out_of_range &e) {
        std::cerr << e.what() << std::endl;
    }

}


bool Site::initCurl(const char *url)
{
    CURLcode code;

    conn = curl_easy_init();

    if(conn == nullptr) {
      fprintf(stderr, "Failed to create CURL connection\n");
      exit(EXIT_FAILURE);
    }

    code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
    if(code != CURLE_OK) {
      fprintf(stderr, "Failed to set error buffer [%d]\n", code);
      return false;
    }

    code = curl_easy_setopt(conn, CURLOPT_URL, url);
    if(code != CURLE_OK) {
      fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);
      return false;
    }

    code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1L);
    if(code != CURLE_OK) {
      fprintf(stderr, "Failed to set redirect option [%s]\n", errorBuffer);
      return false;
    }

    code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
    if(code != CURLE_OK) {
      fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
      return false;
    }

    code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &htmlData);
    if(code != CURLE_OK) {
      fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
      return false;
    }

    return true;
}


int Site::writer(char *data, size_t size, size_t nmemb,
                  std::string *writerData)
{
  if(writerData == NULL)
    return 0;

  writerData->append(data, size*nmemb);

  return size * nmemb;
}

void Scraper::startScraping()
{
    //TODO create a thread

    for( auto site: webSites)
        this->operator()(site.first);
}


void Scraper::operator()(const string & web)
{

}

std::string Scraper::getHeadline(std::string &webSite)
{
        auto it = webSites.find(webSite);

        if(it == webSites.end())
        {
            throw out_of_range("The website you enter is not in the list");
        }

    //Print the headline
}
