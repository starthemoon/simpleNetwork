#include "tcpClient.h"

using std::cout;
using std::cin;
using std::endl;

int main() {
    std::string serverAddr, serverPort;
    cout << "please enter server address:" << endl;
    cin >> serverAddr;
    cout << "please enter server port:" << endl;
    cin >> serverPort;
    tcpClient client(inet_addr(serverAddr.c_str()), (in_port_t)atoi(serverPort.c_str()));
    cin.ignore();
    client.socket_communicate();
    cout << "Client:" << endl;
    cout << "client close normally" << endl;
    return 0;
}
