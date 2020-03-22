#ifndef SERVERCLASS_H
#define SERVERCLASS_H
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <pthread.h>
#include <algorithm>

//these are message codes
//uc/username => user created
//cg/groupname/user#user#... =>created group
//ms/username/message => message send directly one user
//mg/groupname/message =>message send a group

struct ClientList
{
public:
    int socket;
    std::string server_name;
    pthread_t *thread;
    bool operator ==(const ClientList &obj){
        bool flag = this->socket == obj.socket;
        flag = flag && (this->server_name == obj.server_name);
        flag = flag && (this->thread == obj.thread);
        return flag;
    }
    bool operator !=(const ClientList &obj){
        return !this->operator ==(obj);
    }
};
class ServerClass
{
    static ServerClass instance;
    unsigned int port;
    std::string ip;
    int server_fd, addrlen;
    struct sockaddr_in address;
    static std::vector<ClientList*> clients;
    std::vector< std::pair<std::string, std::vector<std::string>>> client_group_info;
    static void *readAndWriteSocket(void *_client);
    void initServer();
    void acceptClients();
    void sendMessageUserCreated(std::string _username);
public:
    explicit ServerClass(const std::string _ip, const unsigned int _port);
    void createGroup(std::string _buffer);
};

#endif // SERVERCLASS_H
