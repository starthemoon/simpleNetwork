#include "tcpServer.h"

int main() {
    tcpServer server;
    std::thread th(server.execute, std::forward<tcpServer *>(&server));
    th.detach();
    std::string str;
    std::cin >> str;
    std::cout << str;
    return 0;
}