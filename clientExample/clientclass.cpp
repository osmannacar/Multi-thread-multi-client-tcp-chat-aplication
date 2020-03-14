#include "clientclass.h"

void ClientClass::initClient()
{
    int sock = 0;
    struct  sockaddr_in serv_addr;
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        puts("socket_failed");
        exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(this->port);
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, this->ip.data(), &serv_addr.sin_addr) <= 0){
        puts("Invalid_address_addres_not_supported");
        exit(EXIT_FAILURE);
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        puts("connection_failed");
        exit(EXIT_FAILURE);
    }


    std::cout << "Enter your client name:" << std::endl;
    char str[1024] = {0};
    gets(str);
    send(sock, str, strlen(str), 0);

    pthread_t read_thread;
    pthread_create(&read_thread, NULL, &readSocket, (void*) &sock);
    pthread_t send_thread;
    pthread_create(&send_thread, NULL, &sendSocket, (void*) &sock);
    (void) pthread_join(read_thread, NULL);
    (void) pthread_join(send_thread, NULL);
}

void *ClientClass::readSocket(void *_socket)
{
    bool flag = true;
    char buffer[1024];
    int *socket = static_cast<int*> (_socket);
    long valread;
    memset(buffer, 0x00, 1024);
    while(flag)
    {
        valread = read(*socket , buffer, 1024);
        if (valread <= 0)
        {
            flag = false;
            break;
        }else {
            puts(buffer);
            memset(buffer, 0x00, valread);
        }

    }
    return NULL;
}

void *ClientClass::sendSocket(void *_socket)
{
    char buffer[1024];
    int *socket = static_cast<int*> (_socket);
    memset(buffer, 0x00, 1024);
    std::string message = std::string("your message :");
    while (true) {
        gets(buffer);
        if(send(*socket, buffer, strlen(buffer), 0) < 0){
            puts("Error while sending data");
            exit(EXIT_FAILURE);
        }
        puts(std::string(message + buffer).data());
        memset(buffer, 0x00, strlen(buffer));
    }
}

ClientClass::ClientClass(const std::string _ip, const unsigned int _port)
{
    this->ip = _ip;
    this->port = _port;
    this->initClient();
}
