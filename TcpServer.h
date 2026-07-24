#pragma once
#include <winsock2.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include "ThreadPool.h"

class TcpServer
{

private:
    SOCKET serverSocket;
    struct sockaddr_in address;
    int port;
    ThreadPool threadPool_;
    void handleClient(SOCKET clientSocket);

public:
    TcpServer(int port);
    void start();
};