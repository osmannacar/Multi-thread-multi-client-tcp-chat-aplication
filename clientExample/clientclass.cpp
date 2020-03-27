#include "clientclass.h"

void ClientClass::initClient(QString _name)
{
    struct  sockaddr_in serv_addr;
    if((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        puts("socket_failed");
        exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(this->port);
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, this->ip.toStdString().data(), &serv_addr.sin_addr) <= 0){
        puts("Invalid_address_addres_not_supported");
        exit(EXIT_FAILURE);
    }
    if (::connect(this->sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        puts("connection_failed");
        exit(EXIT_FAILURE);
    }
    this->sendSocket(_name);
    ClientClassSocket *clientSocket = new ClientClassSocket;
    clientSocket->socket = &(this->sock);
    clientSocket->obje = this;


    pthread_t read_thread;
    pthread_create(&read_thread, NULL, &readSocket, (void*)(clientSocket));
    //    pthread_join(read_thread, NULL);
}

void *ClientClass::readSocket(void *_socket)
{
    bool flag = true;
    char buffer[1024];
    ClientClassSocket *clientSocket = static_cast<ClientClassSocket*> (_socket);
    long valread;
    memset(buffer, 0x00, 1024);
    while(flag)
    {
        valread = read(*(clientSocket->socket) , buffer, 1024);
        if (valread <= 0)
        {
            flag = false;
            break;
        }else {
            clientSocket->obje->readedDataFromSocket(buffer);
            memset(buffer, 0x00, valread);
        }

    }
    return NULL;
}

void ClientClass::sendSocket(QString buffer)
{
    if(send(this->sock, buffer.toStdString().data(), strlen(buffer.toStdString().data()), 0) < 0){
        puts("Error while sending data");
        exit(EXIT_FAILURE);
    }
}

void ClientClass::readedDataFromSocket(QString _incomingtext)
{
    if(_incomingtext == QString("Welcome")){
        this->welcomeMessage();
    }else {
        QString code = _incomingtext.mid(0, 3);
        _incomingtext = _incomingtext.remove(code);
        if(code == QString("uc/")){
            this->userJoin(_incomingtext);
        }else if (code == QString("ul/")) {
            this->userLeft(_incomingtext);
        }else if (code == QString("cg/")) {
            QStringList list = _incomingtext.split("/");
            QString groupname= list.at(0);
            list = list[1].split("#");
            list.removeLast();
            this->createdGroup(groupname, list);
        }else if (code == QString("ms/")) {
            QStringList list = _incomingtext.split("/");
            this->incomingUserMessage(list.at(0), list.at(1));
        }else if (code == QString("mg/")) {
            QStringList list = _incomingtext.split("/");
            this->incomingGroupMessage(list.at(0), list.at(1), list.at(2));
        }else if (code == QString("ou/")) {
            QStringList list = _incomingtext.split("#");
            list.removeLast();
            this->oldUser(list);
        }
    }
}

ClientClass::ClientClass(const QString _ip, const QString _name, const unsigned int _port)
{
    this->ip = _ip;
    this->port = _port;
    this->initClient(_name);
}

void ClientClass::createGroup(QString _groupname, QStringList _userlist)
{
    QString buffer = QString("cg/");
    buffer.append(_groupname);
    buffer.append(QString("/"));
    for (int i = 0; i < _userlist.size(); ++i) {
        buffer.append(_userlist.at(i));
        buffer.append(QString("#"));
    }
    this->sendSocket(buffer);
}

void ClientClass::outgoingUserMessage(QString _username, QString _message)
{
    QString buffer = QString("ms/");
    buffer.append(_username);
    buffer.append(QString("/"));
    buffer.append(_message);
    this->sendSocket(buffer);
}

void ClientClass::outgoingGroupMessage(QString _groupname, QString _message)
{
    QString buffer = QString("mg/");
    buffer.append(_groupname);
    buffer.append(QString("/"));
    buffer.append(_message);
    this->sendSocket(buffer);
}

void ClientClass::outgoingUserName(QString _username)
{
    this->sendSocket(_username);
}
