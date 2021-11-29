#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <netinet/in.h>

#include <iostream>

#include <arpa/inet.h>

#include <unistd.h>

#include <strings.h> // bzero

#include <memory.h> // memset

class tcpClient {
private:
    sockaddr_in serverAddr = {0};
    int socketFd;
public:
    // tcpClient();
    tcpClient(in_addr_t ip, in_port_t p);
    bool socket_start();
    void socket_communicate();
    void socket_close();
    virtual ~tcpClient();
};

#endif