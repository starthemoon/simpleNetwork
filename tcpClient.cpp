#include "tcpClient.h"

using std::cout;
using std::cin;
using std::endl;

// client must provide the ip address and port of server
tcpClient::tcpClient(in_addr_t address, in_port_t port) {
    // bzero(&serverAddr, sizeof(serverAddr));
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = address;

    socketFd = socket(PF_INET, SOCK_STREAM, 0);
    if (socketFd == -1) {
        cout << "Create Error: " << endl;
        cout << "create socketFd failed" << endl;
    }
}

bool tcpClient::socket_start() {
    // convert ip address to string
    char buf[1024] = { 0 };
    if (inet_ntop(AF_INET, &(serverAddr.sin_addr), buf, sizeof(buf)) == nullptr) {
        cout << "Start Error: " << endl;
        cout << "convert ip address to string failed" << endl;
    }

    // connect
    int times = 3;
    while (times--) {
        if (connect(socketFd, (sockaddr*)&serverAddr, sizeof(sockaddr)) == -1) {
            cout << "Start Error: " << endl;
            cout << 3-times << "th connect failed" << endl;
            // usleep(1000000); // sleep for 1s, disabled if using cout
        } else {
            break;
        }
    }
    if (times>= 0) {
        cout << "Start: " << endl;
        cout << "client connects to " << buf <<
            " at port " << ntohs(serverAddr.sin_port) << endl;
    }
    return times >= 0;
}

void tcpClient::socket_communicate() {
    if (socket_start()) {
        char sendData[1024] = { 0 };
        char recvData[1024] = { 0 };
        while (true) {
            memset(sendData, 0, sizeof(sendData));
            memset(recvData, 0, sizeof(recvData));
            cout << "please enter data to send:" << endl;
            cin.getline(sendData, sizeof(sendData));
            // send data
            if (send(socketFd, sendData, strlen(sendData), 0) == -1) {
                cout << "Communicate Error: " << endl;
                cout << "send data failed" << endl;
            }
            cout << "Communicate: " << endl;
            cout << "send data: " << sendData << endl;
            // 'close' for terminate
	        if (strcmp("close", sendData) == 0) break;
            // receive data
            int recvRes = recv(socketFd, recvData, 1024, 0);
            if (recvRes == -1) {
                cout << "Communicate Error: " << endl;
                cout << "receive data failed" << endl;
            } else if (recvRes == 0) {
                cout << "Communicate Error: " << endl;
                cout << "receive data failed" << endl;
            }
            cout << "Communicate: " << endl;
            cout << "receive data: " << recvData << endl;
        }
        socket_close();
    }
}

void tcpClient::socket_close() {
    // close
    if (close(socketFd) == -1) {
        cout << "Close Error: " << endl;
        cout << "socketFd close failed" << endl;
        exit(1);
    }
    cout << "Close: " << endl;
    cout << "socket close normally" << endl;
}

tcpClient::~tcpClient() {
    memset(&serverAddr, 0, sizeof(serverAddr));
}
