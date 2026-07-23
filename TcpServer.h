#pragma once
#include <winsock2.h>
#include <iostream>
#include <stdexcept>
#include <string>

class TcpServer
{

private:
    SOCKET serverSocket;
    struct sockaddr_in address;
    int port;

public:
    TcpServer(int port);
    void start();
    void handleClient(SOCKET clientSocket);
};