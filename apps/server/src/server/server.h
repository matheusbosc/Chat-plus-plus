//
// Created by matheus on 1/27/26.
//

#ifndef CHATPP_SERVER_H
#define CHATPP_SERVER_H

#pragma once

#include <string>
#include <thread>
#include <functional>

struct client {
    std::string clientName;
    int clientSocket;
    std::string room;
    pthread_t clientThread;
    bool threadRunning{false};
    bool threadCreated{false};
};

class Server {
public:
    void Init(bool _uiActive = true);

    int start_server(uint16_t port);
    void stop_server();

    void start_server_listener();
    void stop_server_listener();

    void stop_client_listener();

    ~Server();

private:

    std::vector<client> clients;

    void server_listener_loop();

    void client_listener(client &this_client, std::vector<client> &clients);

    static void* server_listener_entry(void* arg) {
        Server* self = static_cast<Server*>(arg);
        self->server_listener_loop();
        return nullptr;
    }

    static void* client_listener_entry(void* arg) {
        Server* self = static_cast<Server*>(arg);
        self->client_listener(self->clients.back(), self->clients);
        return nullptr;
    }

    bool uiActive = true;


    // internal helpers
    void send_raw(const std::string& json);

    std::atomic<bool> serverListenerRunning{false};
    bool serverListenerCreated{false};
    pthread_t serverListenerThread;

    int serverSocket;

};


#endif //CHATPP_SERVER_H