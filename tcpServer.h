#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h> // sockaddr_in

#include <arpa/inet.h> // inet_pton

#include <iostream>

#include <unistd.h> // close

#include <strings.h> // bzero

#include <memory.h> // memset

#include <errno.h> // errno

// #include <fcntl.h> // fcntl

#include <signal.h> // sigset

#include <thread>  // thread

class tcpServer;

class tcpServer {
private:
    sockaddr_in serverAddr;
    int socketFd;
#define listenNum 128
public:
    tcpServer(in_port_t port = 10086);
    static void execute(tcpServer *);
    void socket_close();
    virtual ~tcpServer();
    static void socket_communicate(int, std::string);
};

#endif