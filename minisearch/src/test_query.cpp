#include "query.h"
#include "EchoServer.h"
#include <iostream>

using namespace std;
int main(int argc, const char *argv[])
{
    EchoServer server(InetAddress(8888));
    server.readfile("../data/index2.txt",
            "../data/reverse_index.txt",
            "../data/pagelib2.txt",
            "../data/doc_wei.txt");
    cout << "read to map over" << endl;
    server.start();
    return 0;
}
