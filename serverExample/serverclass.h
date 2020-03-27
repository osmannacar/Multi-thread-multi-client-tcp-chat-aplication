#ifndef SERVERCLASS_H
#define SERVERCLASS_H
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <algorithm>
#include "clientlist.h"
#include <map>

//these are message codes
//uc/username => user created
//cg/groupname/user#user#... =>created group
//ms/username/message => message send directly one user
//mg/groupname/message =>message send a group

class ServerClass
{
    unsigned int port;
    std::string ip;
    int server_fd, addrlen;
    pthread_mutex_t lock;
    struct sockaddr_in address;
    std::vector<ClientList*> clients;
    std::multimap<std::string, std::string> client_group_info;
    static void *readAndWriteSocket(void *_obje_client);
    void initServer();
    void acceptClients();
    void sendOldUsersToNew(int socket);
    void sendMessageUserCreated(std::string _username);
    void sendMessageUserLeft(std::string _username);
    std::vector<std::string> createGroup(std::string _buffer);
    void removeFromGroups(std::string _user);
public:
    explicit ServerClass(const std::string _ip, const unsigned int _port);
};

struct ObjeClient
{
    ServerClass *obje;
    ClientList *client;
};

#endif // SERVERCLASS_H
