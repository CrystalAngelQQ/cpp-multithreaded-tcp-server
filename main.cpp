#include <iostream>
#include "WsaSession.h"
#include "TcpServer.h"

int main()
{
    try
    {
        std::cout << "Starting TCP Server..." << std::endl;

        WsaSession wsaSession;

        TcpServer tcpServer(8808);
        tcpServer.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}