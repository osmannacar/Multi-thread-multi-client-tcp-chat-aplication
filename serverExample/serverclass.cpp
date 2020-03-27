#include "serverclass.h"
void *ServerClass::readAndWriteSocket(void *_obje_client)
{
    bool flag = true;
    char buffer[1024] = {0};
    std::string buffer_send;
    memset(buffer, 0x00, 1024);
    while (flag) {
        ObjeClient *objeClient = static_cast<ObjeClient*> (_obje_client);
        int valread = read(objeClient->client->socket, buffer, 1024);
        if (valread < 0){
            puts("ERROR reading from socket");
            flag = false;
            break;
        }else if(valread == 0) {
            close(objeClient->client->socket);
            puts(std::string(objeClient->client->user_name +(" has been removed")).data());
            objeClient->obje->removeFromGroups(objeClient->client->user_name);
            std::string username = objeClient->client->user_name;
            objeClient->obje->clients.erase(std::remove(objeClient->obje->clients.begin(), objeClient->obje->clients.end(), objeClient->client), objeClient->obje->clients.end());
//            delete objeClient->client;
            objeClient->obje->sendMessageUserLeft(username);
            pthread_cancel(*objeClient->client->thread);
//            delete objeClient;
            flag = false;
            break;
        }else {
            std::string info = objeClient->client->user_name + " send message :" + buffer;
            puts(info.data());
            //create group
            if(buffer[0] == 'c' && buffer[1] == 'g'){
                std::vector<std::string> group_info = objeClient->obje->createGroup(buffer);
                std::vector<std::string>::iterator itr_group_user;
                std::vector<ClientList*>::iterator it = objeClient->obje->clients.begin();
                while (it != objeClient->obje->clients.end()) {
                    if((*it) != objeClient->client){
                        itr_group_user = std::find(group_info.begin(), group_info.end(), (*it)->user_name);
                        if(itr_group_user != group_info.end()){
                            send((*it)->socket, buffer, strlen(buffer), 0);
                        }
                    }
                    ++it;
                }
            }//send message only one user
            else if(buffer[0] == 'm' && buffer[1] == 's') {
                buffer_send = std::string(buffer);
                if(buffer_send.substr(0, 2) == "ms"){
                    size_t found_first = buffer_send.find_first_of("/");
                    size_t found_last = buffer_send.find_last_of("/");
                    if(found_first != std::string::npos && found_last != std::string::npos ){
                        std::string accepter_name = buffer_send.substr(found_first + 1 , found_last - found_first -1);
                        buffer_send.replace(found_first + 1 , found_last - found_first -1 , objeClient->client->user_name);
                        std::vector<ClientList*>::iterator it = objeClient->obje->clients.begin();
                        while (it != objeClient->obje->clients.end()) {
                            if((*it)->user_name == accepter_name){
                                send((*it)->socket, buffer_send.data(), strlen(buffer_send.data()), 0);
                                break;
                            }
                            ++it;
                        }
                    }

                }

            }//send message to group
            else if (buffer[0] == 'm' && buffer[1] == 'g') {
                buffer_send = std::string(buffer);
                size_t found_first = buffer_send.find_first_of("/");
                size_t found_last = buffer_send.find_last_of("/");
                if(found_first != std::string::npos && found_last != std::string::npos ){
                    std::string group_name = buffer_send.substr(found_first + 1 , found_last - found_first -1);
                    buffer_send.insert(found_last + 1, std::string(objeClient->client->user_name + "/"));

                    std::multimap<std::string, std::string>::iterator itr_group = objeClient->obje->client_group_info.begin();
                    while (itr_group != objeClient->obje->client_group_info.end()) {
                        if(itr_group->first == group_name){
                            std::vector<ClientList*>::iterator it = objeClient->obje->clients.begin();
                            while (it != objeClient->obje->clients.end()) {
                                if((*it) != objeClient->client){
                                    if((*it)->user_name == itr_group->second)
                                        send((*it)->socket, buffer_send.data(), strlen(buffer_send.data()), 0);
                                }
                                ++it;
                            }
                        }
                        ++itr_group;
                    }

                }

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
            sleep(1);//wait for send
            this->sendOldUsersToNew(new_socket);
            std::cout << buffer << ": joined chat" << std::endl;
            //send message to all user that is releated someone join the chat
            if (pthread_mutex_init(&lock, NULL) != 0)
            {
                puts("mutex init failed");
                exit(EXIT_FAILURE);
            }
            this->sendMessageUserCreated(buffer);

            ClientList *client = new ClientList;
            client->user_name = buffer;
            client->socket = new_socket;;
            client->thread = new pthread_t;
            this->clients.push_back(client);
            memset(buffer, 0x00, strlen(buffer));
            ObjeClient *objeClient = new ObjeClient;
            objeClient->client = client;
            objeClient->obje = this;

            //todo mistake
            pthread_join(*client->thread, NULL);
            pthread_create(client->thread, NULL, ServerClass::readAndWriteSocket, static_cast<void *>(objeClient));
            pthread_mutex_destroy(&lock);
        }
    }
}

void ServerClass::sendOldUsersToNew(int socket)
{
    std::string buffer_send = "ou/";
    bool flag = false;
    std::vector<ClientList*>::iterator it = this->clients.begin();
    while (it != this->clients.end()) {
        buffer_send.append((*it)->user_name);
        buffer_send.append("#");
        if(!flag)
            flag = true;
        ++it;
    }
    if(flag)
        send(socket, buffer_send.data(), strlen(buffer_send.data()), 0);
}
void ServerClass::sendMessageUserCreated(std::string _username)
{
    std::string buffer_send = "uc/" + _username;
    std::vector<ClientList*>::iterator it = this->clients.begin();
    while (it != this->clients.end()) {
        send((*it)->socket, buffer_send.data(), strlen(buffer_send.data()), 0);
        ++it;
    }
}

void ServerClass::sendMessageUserLeft(std::string _username)
{
    std::string buffer_send = "ul/" + _username;
    std::vector<ClientList*>::iterator it = this->clients.begin();
    while (it != this->clients.end()) {
        send((*it)->socket, buffer_send.data(), strlen(buffer_send.data()), 0);
        ++it;
    }
}
std::vector<std::string> ServerClass::createGroup(std::string _buffer)
{
    std::string group_name;
    std::vector<std::string> users;
    pthread_mutex_lock(&lock);
    if(_buffer.substr(0, 2) == "cg"){
        _buffer.erase(0, 3);
        size_t found = _buffer.find("/");
        if(found != std::string::npos){
            group_name = _buffer.substr(0, found);
            _buffer.erase(0, found + 1);
            found = _buffer.find("#");
            while (found != std::string::npos) {
                this->client_group_info.insert(std::make_pair(group_name, _buffer.substr(0, found)));
                users.push_back(_buffer.substr(0, found));
                _buffer.erase(0, found + 1);
                found = _buffer.find("#");
            }
        }
    }
    pthread_mutex_unlock(&lock);
    return users;
}

void ServerClass::removeFromGroups(std::string _user)
{
    //todo hata
    pthread_mutex_lock(&lock);
    std::multimap<std::string, std::string>::iterator itr = this->client_group_info.begin();
    while (itr !=  this->client_group_info.end()) {
        if(itr->second == _user)
            this->client_group_info.erase(itr);
        ++itr;
    }
    pthread_mutex_unlock(&lock);
}


