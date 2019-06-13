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
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
using namespace std;

#define PORT 8080
void test(int joj, string s);

void* server(void*) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
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
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        valread = read( new_socket , buffer, 2048);
        QString s(buffer);
        qDebug() << s << endl;




       test(new_socket, s.toStdString());



        //send(new_socket ,  , 400 , 0 );
        // printf("Hello message sent\n");
        fflush(stdout);
        close(new_socket);
    }
}

void test(int new_socket, string s) {
    struct addrinfo hints, *res;
        int sockfd;

        char buf[20560];
        long int byte_count;

          //get host info, make socket and connect it
          memset(&hints, 0,sizeof hints);
          hints.ai_family=AF_UNSPEC;
          hints.ai_socktype = SOCK_STREAM;
          getaddrinfo("www.example.com","80", &hints, &res);
          sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
          printf("Connecting...\n");
          connect(sockfd,res->ai_addr,res->ai_addrlen);
          printf("Connected!\n");
          string header = s;
          send(sockfd,header.c_str(),header.size(),0);
          printf("GET Sent...\n");
          //all right ! now that we're connected, we can receive some data!
          byte_count = recv(sockfd,buf,sizeof(buf)-1,0); // <-- -1 to leave room for a null terminator
          buf[byte_count] = 0; // <-- add the null terminator
          printf("recv()'d %ld bytes of data in buf\n",byte_count);
          printf("%s",buf);

          send(new_socket , buf , (unsigned long)byte_count , 0 );
          printf("acabou\n");
          return;
}
