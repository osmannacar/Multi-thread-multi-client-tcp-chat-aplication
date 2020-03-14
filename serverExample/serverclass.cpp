#include "serverclass.h"
std::vector<ClientList*> ServerClass::clients;
void *ServerClass::readAndWriteSocket(void *_client)
{
    bool flag = true;
    char buffer[1024] = {0};
    std::string buffer_send;
    std::string str = " send ";
    memset(buffer, 0x00, 1024);
    while (flag) {
        ClientList *client = static_cast<ClientList*> (_client);
        int valread = read(client->socket, buffer, 1024);
        if (valread < 0){
            puts("ERROR reading from socket");
            flag = false;
            break;
        }else if(valread == 0) {
            close(client->socket);
            puts(std::string(client->server_name +(" has been removed")).data());
            clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
            pthread_cancel(*client->thread);
            flag = false;
            break;
        }else {
            std::string info= client->server_name + " send message :" + buffer;
            puts(info.data());
            std::vector<ClientList*>::iterator it = ServerClass::clients.begin();
            while (it != ServerClass::clients.end()) {
                if((*it) != client){
                    buffer_send = client->server_name.data() + str + buffer;
                    send((*it)->socket, buffer_send.data(), strlen(buffer_send.data()), 0);
                }
                ++it;
            }
            memset(buffer, 0x00, valread);
        }
    }
    return NULL;
}
void ServerClass::initServer()
{
    int opt = 1;
    // creating socket
    if((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        puts("Error while Creating socket");
        exit(EXIT_FAILURE);
    }
    //     Forcefully attaching socket to the port Port
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        puts("Error while attaching socket");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(this->ip.data());
    address.sin_port = htons(this->port);
    // forcefully attaching socket to the port
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        puts("Error while binding server");
        exit(EXIT_FAILURE);
    }

    // -1 ?
    if(listen(server_fd, -1) < 0){
        puts("Error while listening  server");
        exit(EXIT_FAILURE);
    }
    this->acceptClients();
}

ServerClass::ServerClass(const std::string _ip, const unsigned int _port)
{
    this->ip = _ip;
    this->port = _port;
    this->initServer();
}


void ServerClass::acceptClients()
{
    while (true) {
        char buffer[256] = {0};
        addrlen = sizeof(address);
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
            send(new_socket, "Welcome", 7, 0);
            std::cout << buffer << ": joined chat" << std::endl;

            ClientList *client = new ClientList;
            client->server_name = buffer;
            client->socket = new_socket;;
            client->thread = new pthread_t;
            this->clients.push_back(client);
            memset(buffer, 0x00, strlen(buffer));
            (void) pthread_join(*client->thread, NULL);
            pthread_create(client->thread, NULL, ServerClass::readAndWriteSocket, static_cast<void *>(client));


        }
    }
}


