#include "EchoServer.h"
#include <iostream>
#include <string>
#include <ctype.h>
using namespace std;
using namespace placeholders;
    EchoServer::EchoServer(const InetAddress &addr)
:server_(addr),
    pool_(1000, 4)
{
    server_.setConnection(bind(&EchoServer::onConnection,
                this,
                _1));
    server_.setMessage(bind(&EchoServer::onMessage,
                this,
                _1));
    server_.setClose(bind(&EchoServer::onClose,
                this,
                _1));
}

void EchoServer::readfile(const string& offsetfile,
        const string& reverseindexfile,
        const string& docfile,
        const string& doc_weifile)
{
    qu_.read_to_map(offsetfile, 
            reverseindexfile,
            docfile,
            doc_weifile);
}


void EchoServer::start()
{
pool_.start();
server_.start();
}

void EchoServer::onConnection(const TcpConnectionPtr &conn)
{
    cout << conn->getPeerAddr().toIp() << "port"
        << conn->getPeerAddr().toPort() << endl;   
    conn->send("hello, welcome to Echo Server\n");
}

void EchoServer::onMessage(const TcpConnectionPtr &conn)
{
    string s(conn->receive());
    pool_.addTask(bind(&EchoServer::onCompute, this, s, conn));
}

void EchoServer::onCompute(const std::string &word,
        const TcpConnectionPtr &conn)
{
    string s;
    string word1 = word;
    word1.erase(word1.size()-2, 2);
    s = qu_.search(word1);
    conn->send(s);
}

void EchoServer::onClose(const TcpConnectionPtr &conn)
{
    cout << conn->getPeerAddr().toIp() << " port:"
        << conn->getPeerAddr().toPort() << " close" << endl;
    conn->shutdown();
}






