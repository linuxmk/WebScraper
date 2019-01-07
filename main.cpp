#include <iostream>
#include "scraper.h"

using namespace std;



int main(int argc, char *argv[])
{

    char ch;
    Scraper sc;


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
                try {
                    sc.getHeadline(webSite);
                } catch (out_of_range &e) {
                    cerr << e.what() << endl;
                }
            }
            break;

            case 'e':
                exit(1);

//            default:
//            cerr << "Wrong command..." << endl;
        }


    }

    return 0;
}
