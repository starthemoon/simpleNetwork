#include "tcpServer.h"

using std::cout;
using std::cin;
using std::endl;

// constructor
// using 10086 as default port
tcpServer::tcpServer(in_port_t port) {
    /*
     * create sockaddr_in
     * @term:
     * HBO: host byte order, differ in different platforms, e.g. little-endian in intel x86.
     * NBO: network byte order, big-endian.
     * 
     * @member:
     * sin_family - address family
     * sin_port   - port number, using NBO. Thus conversion from HBO to NBO is needed
     * sin_addr   - a struct with member s_addr to record ip address NBO
     */
    // sockaddr_in: address family, port and address
    // bzero(&serverAddr, sizeof(serverAddr));
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; // address family
    serverAddr.sin_port = htons(port); // port
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // internet address

    /*
     * create unnamed socket
     * @param:
     * __domain   - tell the system which bottom protocol family is used
     * __type     - specify the type of serve
     * __protocol - choose a specific protocol, however, 0 is always filled to use the default
     */
    socketFd = socket(PF_INET, SOCK_STREAM, 0);
    if (socketFd == -1) {
        cout << "Create Error: " << endl;
        cout << "create socketFd failed" << endl;
    }

    /*
     * create named socket by binding an unnamed socket with a given sockaddr_in(ip and port)
     * @param:
     * __fd   - socketFd, the unnamed socket
     * __addr - the sockaddr, which must be type sockaddr for historic reason
     * __len  - the length of __addr
     */
    if (bind(socketFd, (sockaddr*)&serverAddr, sizeof(sockaddr)) == -1) {
        cout << "Create Error: " << endl;
        cout << "create binded socketFd failed" << endl;
    }

    /*
     * create a message queue for listening before acception of clients
     * @param:
     * __fd - the named socket
     * __n  - the length of the message queue, only less than n connections will be queued, others will be refused
     */
    if (listen(socketFd, listenNum) == -1) {
        cout << "Create Error: " << endl;
        cout << "create message listening queue failed" << endl;
    }
    cout << "Create: " << endl;
    cout << "server " << serverAddr.sin_addr.s_addr << " listening at port " <<
        ntohs(serverAddr.sin_port) << endl;

    /*
     * set the sockefFd as nonblock
     * @param:
     * __fd  - open file descriptor
     * __cmd - a specified action, get or set or duplicate
     */
    // fcntl(socketFd, F_SETFL, fcntl(socketFd, F_GETFL) | O_NONBLOCK);
}

// start receive connection
void tcpServer::execute(tcpServer *server) {
    char clientIp[1024] = { 0 };
    while (1) {
        /*
         * take a connection message from the message queue, return a new socket with the connection
         * @param:
         * __fd       - the socket listening to connection message
         * __addr     - the client's address, if not needed can set to be nullptr
         * __addr_len - length of the incoming address, if not needed can set to be nullptr
         */
        sockaddr_in conn;
        socklen_t len = sizeof(sockaddr);
        /*
            * set timer, if the accept block for a time longer than 10s, close socket
            * sigset
            * @param:
            * __sig  - a signal, like SIGALRM, SIGSTOP, SIGKILL
            * __disp - a handler of corresponding signal of the caller process
            * alarm, not so accurate
            * @param:
            * __seconds: seconds before the signal is ejected
            */
        // inline void alarm_handler(int a) {
        //     cout << "alarm_handler receive " << a << endl;
        // };
        // sigset(SIGALRM, [](int a){cout << "alarm_handler receive " << a << endl;});
        // alarm(10);
        int connSocket = accept(server->socketFd, (sockaddr*)&conn, &len);
        if (connSocket < 0) {
            cout << "Execute Error: " << endl;
            cout << "accept the client's connection failed" << endl;
        } else {
            cout << "Execute: " << endl;
            cout << "accept new connection" << endl;
        }
        if (inet_ntop(AF_INET, &(conn.sin_addr), clientIp, 1024) == nullptr) {
            cout << "Execute Error: " << endl;
            cout << "convert ip address to string failed" << endl;
            exit(5);
        }
        std::thread th(socket_communicate, connSocket, std::string(clientIp));
        th.detach();
    }
}

// close
void tcpServer::socket_close() {
    // close socketFd
    if (close(socketFd) == -1) {
        cout << "Close Error:" << endl;
        cout << "socketFd close failed" << endl;
        exit(9);
    }
    cout << "Close:" << endl;
    cout << "socket closed" << endl;
}

// destructor
tcpServer::~tcpServer() {
    socket_close();
    serverAddr = {0};
}

void tcpServer::socket_communicate(int connSocket, std::string clientIp) {
    char sendData[1024] = { 0 }, recvData[1024] = { 0 };
    while (1) {
        // receive data
        memset(sendData, 0, sizeof(sendData));
        memset(recvData, 0, sizeof(recvData));
        if (recv(connSocket, recvData, sizeof(recvData), 0) == -1) {
            cout << "Communicate Error: " << endl;
            cout << "receive data failed" << endl;
            exit(7);
        }
        cout << "Communicate: " << endl;
        cout << "receive data: " << recvData << endl;
        // close socket
        if (strcmp("close", recvData) == 0) break;
        // send data
        strcat(sendData, ": Hello, ");
        strcat(sendData, clientIp.c_str());
        strcat(sendData, ". Your message is received");
        if (send(connSocket, sendData, strlen(sendData), 0) == -1) {
            cout << "Communicate Error:" << endl;
            cout << "send data failed" << endl;
            exit(6);
        }
        cout << "Communicate:" << endl;
        cout << "send data: " << sendData << endl;
    }
    // close connSocket
    if (close(connSocket) == -1) {
        cout << "Communicate Error:" << endl;
        cout << "connSocket close failed" << endl;
        exit(8);
    }
    cout << "Communicate:" << endl;
    cout << "connSocket close normally" << endl;
}