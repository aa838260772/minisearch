#ifndef _ECHOSERVER_H_
#define _ECHOSERVER_H_

#include <echo/TcpServer.h>
#include <echo/ThreadPool.h>
#include <echo/NonCopyable.h>
#include "query.h"
#include <fstream>
#include <iostream>

class EchoServer : private NonCopyable
{
    public:
        EchoServer(const InetAddress &addr);
        void readfile(const std::string&,
                      const std::string&,
                      const std::string&,
                      const std::string&);
        void start();
    private:
        void onConnection(const TcpConnectionPtr &conn);
        void onMessage(const TcpConnectionPtr &conn);
        void onClose(const TcpConnectionPtr &conn);
        void onCompute(const std::string &,
                       const TcpConnectionPtr &);
       
        TcpServer server_;
        ThreadPool pool_;
        query qu_;
};


#endif /*ECHOSERVER_H*/ 
