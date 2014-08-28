#include "pagelib.h"
#include <iostream>
using namespace std;
int main(int argc, const char *argv[])
{
    pagelib pl("../data/yuliao/", "../data/pagelib.txt");
    cout << "read file begin" << endl;
    pl.readdirname();
    cout << "read file end" << endl;

    cout << "begin to store to lib" << endl;
    pl.store_to_lib("../data/index.txt");
    cout << " store wo lib over" << endl;
    return 0;
}
