#ifndef NETWORKSERVER_SERVER_H
#define NETWORKSERVER_SERVER_H

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include "Socket.h"
#include <thread>
#include <chrono>
#include <atomic>
#include <math.h>
#pragma comment (lib, "Ws2_32.lib")
using namespace std::chrono_literals;

class Server {

public:
    Server(std::string port);

    Socket *getConnection();

    void handleRequest(Socket *socket, Server* server);

    static void destroy();
    std::atomic<int> numberOfOpenConnections{0};
private:
    static const int MAX_WAITING_TIME = 26;
    SOCKET listenSocket = INVALID_SOCKET;

};


#endif //NETWORKSERVER_SERVER_H
