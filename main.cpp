#include <iostream>
#include <assert.h>
using namespace std;

int main(int argc, char *argv[])
{

   char ch,x;

    while (true) {

        cerr << "press e to exit===>";
        cin >> ch;

        switch (ch) {
            case 's':
            break;

            case 'h':
            break;

            case 'e':
                exit(1);

            default:
            cerr << "Wrong command..." << endl;
        }


    }

    return 0;
}
