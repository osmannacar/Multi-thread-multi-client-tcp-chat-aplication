#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <map>
#include <pthread.h>
static pthread_mutex_t mutex;
#define PORT 1337

static std::map<int, std::string> clients;
static int server_fd, addrlen;
static struct sockaddr_in address;

void *readAndWrite(void *_new_socket){
    bool flag = true;
    char buffer[256] = {0};
    while (flag) {
        int new_socket = *static_cast<int*> (_new_socket);
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, 1024);
        if (valread < 0){
            puts("ERROR reading from socket");
            flag = false;
            break;
        }else if(valread == 0) {
            close(new_socket);
            clients.erase(new_socket);
            flag = false;
            break;
        }else {
            std::cout << clients.find(new_socket)->second << " send message :" << buffer << std::endl;
            std::map<int, std::string>::iterator it = clients.begin();
            while (it != clients.end()) {
                if(it->second != clients.find(new_socket)->second){
                    send(it->first, buffer, strlen(buffer), 0);
                }
                ++it;
            }
        }
    }
    return NULL;
}
void *acceptClient(){
    while (true) {
        char buffer[256] = {0};
        addrlen = sizeof(address);
        pthread_t client_th;
        int new_socket = (accept(server_fd,(struct sockaddr *)&address, (socklen_t*)&addrlen));
        if(new_socket < 0){
            puts("Error while Accepting Socket");
            exit(EXIT_FAILURE);
        }
        int valread = read(new_socket, buffer, 1024);
        if (valread < 0){
            puts("ERROR reading from socket");
            exit(EXIT_FAILURE);
        }else if( valread == 0) {
            close(new_socket);
        }else {
            std::cout << buffer << ": joined chat" << std::endl;
            clients.insert(std::make_pair(new_socket, buffer));
            pthread_create(&client_th, NULL, &readAndWrite, static_cast<void *>(&new_socket));
        }
    }
    return NULL;
}
void initServer(){
    int opt = 1;

    // creating socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        puts("Error while Creating socket");
        exit(EXIT_FAILURE);
    }

    //     Forcefully attaching socket to the port Port
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        puts("Error while attaching socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    // forcefully attaching socket to the port 8080
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        puts("Error while binding server");
        exit(EXIT_FAILURE);
    }

    // -1 ?
    if(listen(server_fd, -1) < 0){
        puts("Error while listening  server");
        exit(EXIT_FAILURE);
    }
    acceptClient();
}
int main()
{
    std::cout << "\033]0;" << "Server" << "\007" << "Server" << std::endl;
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);
    initServer();
    pthread_mutex_lock(&mutex);

    return 0;
}
