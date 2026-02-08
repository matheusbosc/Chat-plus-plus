//
// Created by matheus on 1/15/26.
//

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#include <thread>
#include <vector>
#include <common_lib/common_lib.h>
#include <json_struct/json_struct.h>
#include "server.h"


void Server::Init(bool _uiActive) {
    uiActive = _uiActive;
}

int Server::start_server(unsigned short int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    //std::cout << serverSocket << std::endl;

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;



    // binding socket.
    if (bind(serverSocket, (struct sockaddr*)&serverAddress,
         sizeof(serverAddress)) < 0) {
        std::cout << "Failed to bind to port " << port << std::endl;
    }

    // listening to the assigned socket
    listen(serverSocket, 5);

    return 0;
}

void Server::start_server_listener() {
    serverListenerRunning = true;
    pthread_create(&serverListenerThread, nullptr, &Server::server_listener_entry, this);
    serverListenerCreated = true;
}

void Server::server_listener_loop() {
    while (true) {
        // accepting connection request
        int clientSocket
            = accept(serverSocket, nullptr, nullptr);

        clients.emplace_back("client", clientSocket);
        client* c = &clients.back();
        c->threadCreated = true;
        c->threadRunning = true;
        pthread_create(&clients.back().clientThread, nullptr, &Server::client_listener_entry, this);
    }
}

void Server::stop_client_listener() {

    for (client c: clients) {

        shutdown(c.clientSocket, SHUT_RDWR);
        close(c.clientSocket);

        c.threadRunning = false;
        pthread_join(c.clientThread, nullptr);
        c.threadCreated = false;
    }

}

void Server::stop_server_listener() {
    shutdown(serverSocket, SHUT_RDWR);
    close(serverSocket);

    serverListenerRunning = false;

    pthread_join(serverListenerThread, nullptr);

    serverListenerCreated = false;
}

void Server::client_listener(client &this_client, std::vector<client> &clients) {
    while (true) {
        // receiving data
        char buffer[1024] = { 0 };
        ssize_t bytes = recv(this_client.clientSocket, buffer, sizeof(buffer), 0);
        if (bytes == 0) {
            return;
        }
        if (bytes < 0) {
            return;
        }

        JS::ParseContext context(buffer);
        common_lib::message obj;
        context.parseTo(obj);
        this_client.room = obj.room_code;

        common_lib::message returnmsg;
        int returnMsgCode;

        // Check if the message contains a default pattern:
        if (obj.type == "server_action") {
            std::cout << "ROOM " << obj.room_code << ": " << "SERVER ACTION: " << obj.author << ": " << obj.content
                  << std::endl;

            returnmsg = common_lib::message(obj.content, "SERVER", obj.room_code, 201, "server_response");

            returnMsgCode = 0;
        } else if (obj.type == "error") {
            std::cout << "ROOM " << obj.room_code << ": " << "ERROR: " << ": " << obj.author << ": " << obj.content
                  << std::endl;

            returnmsg = common_lib::message(obj.content, "ERROR", obj.room_code, 400, "server_response");

            returnMsgCode = -1;
        }

        // Specific Patterns
        

        // If none of the patterns above apply, then return the message
        else {
            std::cout << "ROOM " << obj.room_code << ": " << "OTHER MESSAGE: " << obj.author << ": " << obj.content
                  << std::endl;

            returnmsg = common_lib::message(obj.content, obj.author, obj.room_code, 200, "server_response");

            returnMsgCode = 0;
        }

        std::string json_chat_msg = JS::serializeStruct(returnmsg);

        // sending data
        const char* json_returnmsg = json_chat_msg.c_str();

        if (returnMsgCode == 0) {
            for (int i = 0; i < clients.size(); i++) {

                if (obj.content == "Disconnected" && clients[i].clientSocket == this_client.clientSocket) continue;

                if (clients[i].room != this_client.room) continue;

                send(clients[i].clientSocket, json_returnmsg, strlen(json_returnmsg), 0);
            }
        } else {
            send(this_client.clientSocket, json_returnmsg, strlen(json_returnmsg), 0);
        }

    }
}

void Server::stop_server() {
    stop_client_listener();
    stop_server_listener();
}

Server::~Server() {
}
