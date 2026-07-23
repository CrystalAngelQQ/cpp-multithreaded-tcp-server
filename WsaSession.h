#pragma once
#include <winsock2.h>
#include <iostream>
#include <stdexcept>
#include <string>

class WsaSession {
public:
    WsaSession() {
        WSADATA wsaData;

        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            throw std::runtime_error("WSAStartup failed with error: " + std::to_string(result));
        }
        std::cout << "Winsock initialized successfully." << std::endl;
    }

    ~WsaSession() {
        WSACleanup();
        std::cout << "Winsock cleaned up." << std::endl;
    }

    WsaSession(const WsaSession&) = delete;
    WsaSession& operator=(const WsaSession&) = delete;
};