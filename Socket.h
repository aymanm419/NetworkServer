#ifndef NETWORKSERVER_SOCKET_H
#define NETWORKSERVER_SOCKET_H


#include <ws2tcpip.h>
#include <string>
#include<fcntl.h>
#include "HTTP.h"

class Socket {
public:
    Socket(SOCKET socket);

    void sendString(std::string str);

    HTTP readRequest(int* readStatus) const;

    bool readyToReceive(int timeOut);

    void destroy() const;

    SOCKET getSocket() { return socket; }

private:
    SOCKET socket;


};


#endif //NETWORKSERVER_SOCKET_H
