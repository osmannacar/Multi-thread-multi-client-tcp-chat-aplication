#ifndef CLIENTCLASS_H
#define CLIENTCLASS_H
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

class ClientClass
{
    std::string ip;
    unsigned int port;
    void initClient();
    static void* readSocket(void* _socket);
    static void* sendSocket(void* _socket);
    pthread_mutex_t mutex;
public:
    explicit ClientClass(const std::string _ip, const unsigned int _port);
};

#endif // CLIENTCLASS_H
