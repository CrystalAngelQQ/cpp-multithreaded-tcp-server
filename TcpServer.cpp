#include "TcpServer.h"
#include <iostream>

TcpServer::TcpServer(int port)
{
    this->port = port;

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

        handleClient(clientSocket);
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

        if (bytesReceived <= 0)
        {
            std::cout << "Client disconnected or error." << std::endl;
            break;
        }

        std::cout << "Received from client: " << buffer << std::endl;

        std::string response = "Hello from C++ Server! Message received.\n";
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    closesocket(clientSocket);
}