#include <iostream>
#include "scraper.h"

using namespace std;
static bool forever = true;


int main(int  /*argc*/, char * /*argv*/[])
{

    char ch;

    unordered_map<std::string, WebTitle> webSites;
    const char * attr[] = { "class",
                  "cluster"};

    const char * attrEspn[] = { "class",
                  "contentItem__contentWrapper"};

    const char * attrFoxNews[] = { "class",
                  "content" , "2"};

    const char * attrNBCNews[] = { "class",
                  "info___1Xmsp pt5 pt0 ph5 pl4-m"};

    webSites.insert({
                        {"www.time.mk", WebTitle( std::string("https://time.mk"), std::string("div") , attr )},
                         {"www.espn.com", WebTitle( std::string("www.espn.com"), std::string("div"), attrEspn )},
                         {"www.foxnews.com", WebTitle( std::string("www.espn.com"), std::string("div"), attrFoxNews )},
                     {"www.nbcnews.com", WebTitle( std::string("www.espn.com"), std::string("div"), attrNBCNews )}
                    }
    );

    Scraper sc(webSites);

    while (forever) {

        cerr << "press e to exit===>";
        cin >> ch;


        switch (ch) {
            case 's':
                sc.startScraping();
            break;

            case 'h':
            {
                string webSite;
                cin >> webSite;
                try {
                    sc.getHeadline(webSite);
                } catch (out_of_range &e) {
                    cerr << e.what() << endl;
                }
            }
            break;

            case 'e':
                forever = false;
                break;

//            default:
//            cerr << "Wrong command..." << endl;
        }
    }


    return 0;
}
