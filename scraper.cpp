#include <iostream>
#include <fstream>
#include "scraper.h"

using namespace  std;

void WebTitle::StartElement(void *voidContext,
                         const xmlChar *name,
                         const xmlChar **attributes)
{
    auto *context = (Context *)voidContext;

    if(attributes != nullptr && *attributes != nullptr)
    {
        if( (!context->dataExtracted) && !strcasecmp((char *)context->tag.c_str(),(char *)name ) && !strcasecmp( *(char **)attributes, (char *)context->attr[0]) && !strcasecmp( *(char **)(attributes+1), (char *)context->attr[1] ) )
        {
            cerr << "Attribute[0] = " << attributes[0] << " attr[1] = " << attributes[1] << endl;
            context->title = "";
            context->dataExtracted = true;
            context->addTitle = true;

        }
    }
}

//
//  libxml end element callback function
//

void WebTitle::EndElement(void *voidContext,
                       const xmlChar *name)
{
    auto *context = (Context *)voidContext;

    if(!strcasecmp((char *)name, "div") && context->addTitle )
    {
         context->addTitle = false;
    }
}

//
//  Text handling helper function
//

void WebTitle::handleCharacters(Context *context,
                             const xmlChar *chars,
                             int length)
{
  if(context->addTitle)
  {
    cerr << "chars = " << chars << endl;
    context->title.append((char *)chars, length);
    cerr << "title = " << context->title << endl;
  }
}

//
//  libxml PCDATA callback function
//

void WebTitle::Characters(void *voidContext,
                       const xmlChar *chars,
                       int length)
{
    auto *context = (Context *)voidContext;
    handleCharacters(context, chars, length);
}

//
//  libxml CDATA callback function
//

void WebTitle::cdata(void *voidContext,
                  const xmlChar * chars,
                  int  length)
{
  auto *context = (Context *)voidContext;

  handleCharacters(context, chars, length);
}


void WebTitle::parseHtml()
{
  htmlParserCtxtPtr ctxt;
//  Context context;
  context.title = "";

//  htmlDocPtr doc = htmlReadMemory(htmlData.c_str() , htmlData.size(), "" , nullptr,HTML_PARSE_RECOVER );

//  if(!doc)
//      return;

  ctxt = htmlCreatePushParserCtxt(&saxHandler, &context, "", 0, "",
                                  XML_CHAR_ENCODING_NONE);

  htmlParseChunk(ctxt, htmlData.c_str(), htmlData.size(), 0);
  htmlParseChunk(ctxt, "", 0, 1);

  htmlFreeParserCtxt(ctxt);

}

bool WebTitle::createConn(const std::string &name)
{
    if(!initCurl(name.c_str()))
        throw std::out_of_range("Can not get " + name);

    code = curl_easy_perform(conn);
    curl_easy_cleanup(conn);

    if(code != CURLE_OK) {
      throw std::out_of_range("Can not perform curl_easy_perform " );
    }

    ofstream s;
    s.open("time.mk.out", ios::app);

    s << htmlData << endl;

    return true;
}

WebTitle::WebTitle(const std::string &name, const std::string &tag, const char *attr[])
{
    context.tag = tag;
    context.attr = (char **)attr;

   // createConn( name);
}


Scraper::Scraper(std::unordered_map<std::string, WebTitle> & umap) : webSites(umap)
{

}


Scraper::~Scraper()
{
    for( auto& it : threadIds)
        it.join();
}



bool WebTitle::initCurl(const char *url)
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

    htmlData = "";
    code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &htmlData);
    if(code != CURLE_OK) {
      fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
      return false;
    }

    return true;
}


int WebTitle::writer(char *data, size_t size, size_t nmemb,
                  std::string *writerData)
{
  if(writerData == nullptr)
    return 0;

  writerData->append(data, size*nmemb);

  return size * nmemb;
}

void Scraper::startScraping()
{
    for( auto &site: webSites)
    {

        threadIds.push_back(std::thread([&site]() {

            site.second.createConn(site.first);
            site.second.parseHtml();
        }));
    }
}


std::string Scraper::getHeadline(std::string &webSite)
{
        auto it = webSites.find(webSite);

        if(it == webSites.end())
        {
            throw out_of_range("The website you enter is not in the list");
        }

        cerr << "WebSite << " << it->first  << "HeadLine news =>" << it->second.getContext().title << endl;
        it->second.getContext().dataExtracted = false;
        return it->second.getContext().title;
}
