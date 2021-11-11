#include "Server.h"
#include "File.h"

Server::Server(std::string port) {
    WSADATA wsaData;
    int iResult;
    struct addrinfo *result = nullptr;
    struct addrinfo hints{};


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(nullptr, port.c_str(), &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return;
    }

    // Create a SOCKET for connecting to server
    this->listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (this->listenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Setup the TCP listening socket
    iResult = bind(this->listenSocket, result->ai_addr, (int) result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(this->listenSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(result);
    iResult = listen(this->listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(this->listenSocket);
        WSACleanup();
        return;
    }
}

void Server::handleRequest(Socket *socket, Server *server) {
    int connections = server->numberOfOpenConnections.load(std::memory_order_relaxed);
    int waitingTime = MAX_WAITING_TIME - exp((1.0 * connections * connections) / (90.00 * connections + 10));
    while (socket->readyToReceive(waitingTime)) {
        int readVal = 0;
        HTTP request = socket->readRequest(&readVal);
        if (readVal <= 0)
        {
            std::cout << ">>> Client has gracefully closed the connection" << std::endl;
            break;
        }
        std::cout << ">>> Request" << std::endl;
        std::cout << request.build().getString() << std::endl;
        if (request.getRequestType() == "POST") {
            writeFile(request.getRequestFile(), request.getBody());
            socket->sendString(HTTP().setStatus(200)
                                       .addHeader("Content-Length","0")
                                       .build()
                                       .getString());
        } else if (request.getRequestType() == "GET") {
            if (!fileExists(request.getRequestFile())) {
                std::string fileNotFound = "File Not Found";
                socket->sendString(HTTP()
                                           .setStatus(404)
                                           .addHeader("Content-Type", "text/plain")
                                           .addHeader("Content-Length", std::to_string(fileNotFound.size()))
                                           .addHeader("Connection", "keep-alive")
                                           .setBody(fileNotFound)
                                           .build().getString());
            } else {
                std::string fileContent = getFileContent(request.getRequestFile());
                std::cout << fileContent << std::endl;
                HTTP http = HTTP()
                        .setStatus(200)
                        .addHeader("Content-Length", std::to_string(fileContent.size()))
                        .addHeader("Connection", "keep-alive")
                        .setBody(fileContent);
                if (request.getRequestFile().find(".jpg") != std::string::npos)
                    http.addHeader("Content-Type", "image/jpg");
                else if (request.getRequestFile().find(".png") != std::string::npos)
                    http.addHeader("Content-Type", "image/png");
                socket->sendString(http.build().getString());
            }
        }
        if (request.getHeader("Connection") != "keep-alive")
            break;
    }
    std::cout << ">>> Destroying thread!" << std::endl;
    server->numberOfOpenConnections--;
    socket->destroy();
    delete socket;
}

Socket *Server::getConnection() {
    SOCKET clientSocket = accept(this->listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        printf(">>> accept failed with error: %d\n", WSAGetLastError());
        closesocket(this->listenSocket);
        return nullptr;
    }
    return new Socket(clientSocket);
}

void Server::destroy() {
    WSACleanup();
}
