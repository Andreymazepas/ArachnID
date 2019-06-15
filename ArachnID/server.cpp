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
#include "http_helper.h"

using namespace std;

void do_send(int joj, string s);

void debug(string s) {
    qDebug() << QString(s.c_str()) << endl;
}

int read_all_content_from_socket(int sock_fd,char* buf) {
    recv(sock_fd,buf,1,0);
    int total = 1;
    while(recv(sock_fd,buf+total,1,MSG_DONTWAIT) > 0) {
//        debug("eita " + to_string(read));
        total++;
    }
    return total;
}

QString read_from_socket(int socket) {
    char buffer[1 << 18];
    read_all_content_from_socket(socket, buffer);
    return QString(buffer);
}

int create_server_fd() {
    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)  {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

// Forcefully attaching socket to the port 8080
void attach_socket_to_port(int server_fd,struct sockaddr_in *address, int port = 8080) {
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))  {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons( port );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *) address, sizeof(*address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}


int send_request_to_the_web(string request) {
    struct addrinfo hints, *res;
    int web_sock_fd;
    char buf[1 << 18];
    long int byte_count;

    //get host info, make socket and connect it
    memset(&hints, 0,sizeof hints);
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int get_addr_res = getaddrinfo(getHostFromHTTPRequest(request).c_str(),"80", &hints, &res);
    web_sock_fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    connect(web_sock_fd,res->ai_addr,res->ai_addrlen);
    send(web_sock_fd,request.c_str(),request.size(),0);
    return web_sock_fd;
}

void send_response_to_browser(int browser_socket, QString response) { // ver esses tamanho
    send(browser_socket , response.toStdString().c_str() , (unsigned long)response.size() , 0 );
}

void* server(void* arg) {
    long queueId = *((long*)arg);
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int server_fd = create_server_fd();
    attach_socket_to_port(server_fd, &address);
    if (listen(server_fd, 25) < 0)  {
        perror("listen");
        exit(EXIT_FAILURE);
    }


    while(1) {
        debug("======================================================");
        int browser_socket;
        if ((browser_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        QString request = read_from_socket(browser_socket);
        debug("Incoming request!\n");
        qDebug() << request.toUtf8() << endl;
        int web_sock_fd = send_request_to_the_web(request.toStdString());
        QString response = read_from_socket(web_sock_fd);
        debug("Incoming response!\n");
        qDebug() << response.toUtf8() << endl;;
//        response = "kekekeke";
        send_response_to_browser(browser_socket, response);
        debug("response sent!");
        close(web_sock_fd);
        close(browser_socket);
        debug("closed sockets");
    }
}

