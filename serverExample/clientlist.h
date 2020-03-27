#ifndef CLIENTLIST_H
#define CLIENTLIST_H
#include <string.h>
#include <vector>
#include <pthread.h>
#include <string>
struct ClientList
{
    int socket;
    std::string user_name;
    pthread_t *thread;
    bool operator ==(const ClientList &obj){
        bool flag = this->socket == obj.socket;
        flag = flag && (this->user_name == obj.user_name);
        flag = flag && (this->thread == obj.thread);
        return flag;
    }
    bool operator !=(const ClientList &obj){
        return !this->operator ==(obj);
    }
};
#endif // CLIENTLIST_H
