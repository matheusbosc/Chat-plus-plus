//
// Created by matheus on 1/15/26.
//

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <csignal>
#include <cstdlib>

using namespace std;

int main()
{
    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress,
            sizeof(serverAddress));



    try {

        auto throwFunction = [](int e) {
            throw e;
        };

        signal(SIGINT, throwFunction);

        while(true) {

            string input;
            cout << "enter input: ";
            if (!getline(std::cin, input)) break;

            // sending data
            const char* message = input.c_str();
            send(clientSocket, message, strlen(message), 0);
        }

    } catch (int e) {
        // closing the socket.
        close(clientSocket);
        return 0;
    }

    // closing the socket.
    close(clientSocket);
    return 0;
}