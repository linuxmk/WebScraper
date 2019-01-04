#include <iostream>
#include "scraper.h"

using namespace std;





int main(int argc, char *argv[])
{

   char ch;
   Scraper sc = Scraper::getInstance();

    while (true) {

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
                sc.getHeadline(webSite);
            }
            break;

            case 'e':
                exit(1);

            default:
            cerr << "Wrong command..." << endl;
        }


    }

    return 0;
}
