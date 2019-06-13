// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "pthread.h"
#include <bits/stdc++.h>
#include <QDebug>
using namespace std;

#define PORT 8228


void* server(void* arg) {
    long queueId = *((long*)arg);
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[2048] = {0};
    string hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)  {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))  {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    while(1) {

        if (listen(server_fd, 3) < 0)  {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        qDebug() << address.sin_family << endl;
        qDebug() << address.sin_addr.s_addr << endl;
        qDebug() << address.sin_port << endl;
        qDebug() << new_socket << endl;
        valread = read(new_socket , buffer, 2048);
        QString s(buffer);
        qDebug() << s << endl;
        qDebug() << valread << endl;
        for(int i = 0; i < valread; i += 5) {
            buffer[i] = 'a' + (rand() % 7);
        }
        send(new_socket ,  buffer, valread, 0 );
        qDebug() << "Hello message sent" << endl;
        close(new_socket);
    }
}
