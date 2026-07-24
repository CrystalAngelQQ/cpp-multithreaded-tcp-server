#include "TcpServer.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cstring>

TcpServer::TcpServer(int port) : port(port), threadPool_(4)
{
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET)
    {
        throw std::runtime_error("Failed to create socket");
    }

    std::cout << "Socket created: " << serverSocket << std::endl;
}

void TcpServer::start()
{
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    int bind_value = bind(serverSocket, (struct sockaddr *)&address, sizeof(address));
    if (bind_value < 0)
    {
        perror("Could not bind");
        return;
    }

    int listen_value = listen(serverSocket, SOMAXCONN); // can put a number instead of SOMAXCONN
    if (listen_value == SOCKET_ERROR)
    {
        perror("Could not listen");
        return;
    }
    std::cout << "Server is listening on port " << port << "..." << std::endl;

    while (true)
    {
        struct sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);

        SOCKET clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize);

        if (clientSocket == INVALID_SOCKET)
        {
            perror("Could not accept");
            continue;
        }

        std::string clientIp = inet_ntoa(clientAddress.sin_addr);
        int clientPort = ntohs(clientAddress.sin_port);
        std::cout << "Accepted new client @ " << clientIp << ":" << clientPort << std::endl;

        threadPool_.enqueue([this, clientSocket]()
                            { this->handleClient(clientSocket); });
    }
}

void TcpServer::handleClient(SOCKET clientSocket)
{
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

        if (bytesReceived == 0)
        {
            std::cout << "Client disconnected gracefully.\n";
            break;
        }
        else if (bytesReceived == SOCKET_ERROR)
        {
            std::cerr << "Recv failed with error: " << WSAGetLastError() << "\n";
            break;
        }

        std::string rawMessage(buffer);

        rawMessage.erase(rawMessage.find_last_not_of("\r\n") + 1);

        if (rawMessage.empty())
        {
            continue;
        }

        std::string command;
        std::string argument;

        std::stringstream ss(rawMessage);
        ss >> command;

        std::getline(ss, argument);

        if (!argument.empty() && argument[0] == ' ')
        {
            argument.erase(0, 1);
        }

        std::string response;

        if (command == "PING")
        {
            response = "PONG\n";
        }
        else if (command == "ECHO")
        {
            response = argument + "\n";
        }
        else if (command == "TIME")
        {

            auto now = std::chrono::system_clock::now();
            std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

            char timeBuf[100];
            std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&nowTime));

            response = std::string(timeBuf) + "\n";
        }
        else if (command == "QUIT")
        {
            response = "BYE\n";
            send(clientSocket, response.c_str(), response.size(), 0);
            break;
        }
        else
        {
            response = "UNKNOWN COMMAND\n";
        }

        send(clientSocket, response.c_str(), response.size(), 0);
    }

    closesocket(clientSocket);
}