cmake_minimum_required(VERSION 3.15)
project(cpp-multithreaded-tcp-server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} 
    main.cpp
    WsaSession.h
    TcpServer.h
    TcpServer.cpp
    SafeQueue.h
    ThreadPool.h
)

target_link_libraries(${PROJECT_NAME} ws2_32)