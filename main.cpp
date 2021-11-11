#include "Server.h"
#include <future>
#include "File.h"
using namespace std;

int main() {
    Server server("4600");
    Socket *socket;
    while ((socket = server.getConnection()) != nullptr) {
        std::cout << ">>> New Connection!" << std::endl;
        server.numberOfOpenConnections++;
        std::thread thread(&Server::handleRequest, &server, socket, &server);
        thread.detach();
        std::cout << ">>> Delegated connection to thread" << std::endl;
    }
    return 0;
}
