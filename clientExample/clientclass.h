#ifndef CLIENTCLASS_H
#define CLIENTCLASS_H
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <QtCore>

class ClientClass : public QObject
{
    Q_OBJECT
    QString ip;
    int sock = 0;
    unsigned int port;
    void initClient(QString _name);
    static void* readSocket(void* _socket);
    void sendSocket(QString buffer);
    pthread_mutex_t mutex;
    void readedDataFromSocket(QString _incomingtext);
public:
    explicit ClientClass(const QString _ip, const QString _name, const unsigned int _port);
    void createGroup(QString _groupname, QStringList _userlist);
    void outgoingUserMessage(QString _username, QString _message);
    void outgoingGroupMessage(QString _groupname, QString _message);
    void outgoingUserName(QString _username);
signals:
    void userJoin(const QString &_username);
    void createdGroup(const QString &_groupname, const QStringList &_userlist);
    void userLeft(const QString &_username);
    void incomingUserMessage(const QString &_username, const QString &_message);
    void incomingGroupMessage(const QString &_groupname, const QString &_username, const QString &_message);
    void oldUser(const QStringList &_userlist);
    void welcomeMessage();

};
struct ClientClassSocket
{
    int *socket;
    ClientClass *obje;
};

#endif // CLIENTCLASS_H
