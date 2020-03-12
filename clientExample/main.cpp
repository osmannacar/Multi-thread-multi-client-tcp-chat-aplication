#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 1337
int run = 1;
char buffer[1024];
void* readSocket(void* _socket)
{
    int socket = *static_cast<int*> (_socket);
    long valread;
    while(run)
    {
        memset(buffer, 0, sizeof(buffer));
        valread = read(socket , buffer, 1024);
        if (valread <= 0)
        {
            run = 0;
            break;
        }else {
            std::cout << "Incoming Message: " <<  buffer << std::endl;
            memset(buffer, 0x00, valread);
        }

    }
    return NULL;
}

int main()
{
    std::cout << "\033]0;" << "Client" << "\007" << "Client" << std::endl;
    int sock = 0;
    struct  sockaddr_in serv_addr;
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        puts("socket_failed");
        exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
        puts("Invalid_address_addres_not_supported");
        exit(EXIT_FAILURE);
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        puts("connection_failed");
        exit(EXIT_FAILURE);
    }

    pthread_t read_thread;
    pthread_create(&read_thread, NULL, &readSocket, (void*) &sock);
    std::cout << "Enter your client name:" << std::endl;
    char str[1024] = {0};
    gets(str);
    send(sock, str, strlen(str), 0);
    while (true) {
        memset(str, 0, strlen(str));
        gets(str);
        send(sock, str, strlen(str), 0);
        std::cout << "your message :" << str << std::endl;
    }



    return 0;
}
