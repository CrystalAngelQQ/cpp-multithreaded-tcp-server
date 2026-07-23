#include <iostream>
#include "WsaSession.h"

int main() {
    try {
        std::cout << "Starting TCP Server..." << std::endl;
        
        WsaSession wsaSession; 
        

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}