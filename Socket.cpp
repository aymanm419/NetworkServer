#include "Socket.h"
#include "HTTP.h"
#include <iostream>

#define DEFAULT_BUFLEN 16384

Socket::Socket(SOCKET socket) {
    this->socket = socket;
    ioctlsocket(this->socket, FIONBIO, nullptr);
}

void Socket::sendString(std::string str) {
    int result = send(this->socket, str.c_str(), sizeof(char) * str.size(), 0);
    if (result == SOCKET_ERROR) {
        wprintf(L"send failed with error: %d\n", WSAGetLastError());
    }
}

HTTP Socket::readRequest(int* readStatus) const {
    char recvbuf[DEFAULT_BUFLEN];
    int iResult = -1;
    HTTP request;
    if ((iResult = recv(this->socket, recvbuf, DEFAULT_BUFLEN, MSG_PEEK)) > 0) {
        request = HTTP(recvbuf, iResult);
        int totalBytes = request.build().getString().size();
        recv(this->socket, recvbuf, totalBytes, 0);
        long long bodyLength = 0;
        if(!request.getHeader("Content-Length").empty())
            bodyLength = std::stoll(request.getHeader("Content-Length"));
        std::string body;
        while (bodyLength > 0) {
            long long read = recv(this->socket, recvbuf, std::min(DEFAULT_BUFLEN*1LL, bodyLength), 0);
            for (int i = 0; i < read; i++)
                body += recvbuf[i];
            bodyLength -= DEFAULT_BUFLEN;
        }
        request.setBody(body);
    }
    if(readStatus != nullptr)
        (*readStatus) = iResult;
    return request;
}

void Socket::destroy() const {
    int iResult = closesocket(this->socket);
    if (iResult == SOCKET_ERROR) {
        printf("failed to close socket with error: %d\n", WSAGetLastError());
    }
}

bool Socket::readyToReceive(int timeOut) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(this->socket, &fds);

    timeval tv;
    tv.tv_sec = std::max(3, timeOut);
    tv.tv_usec = 0;

    return (select(this->socket + 1, &fds, 0, 0, &tv) == 1);
}
