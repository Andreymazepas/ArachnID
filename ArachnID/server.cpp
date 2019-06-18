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

char BRBN[4] = {'\r', '\n', '\r', '\n'};

void debug(string s) {
    qDebug() << QString(s.c_str()) << endl;
}

int read_until_terminators(int sock_fd,char* buf, char* terminator, int terminators_size) {
    int total = 0;
    while(1) {
        bool should_break = false;
        if(total >= terminators_size) {
            for(int i = 0; i < terminators_size; i++) {
                if(buf[total - terminators_size + i] != terminator[i]) break;
                if(i == terminators_size - 1) should_break = true;
            }
        }
        if(should_break) break;
        read(sock_fd,buf+total,1);
        total++;
    }
    return total;
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


int send_request_to_the_web(char buffer[]) {
    struct addrinfo hints, *res;
    int web_sock_fd;

    //get host info, make socket and connect it
    memset(&hints, 0,sizeof hints);
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    HTTP_parser::parse(buffer);
    int get_addr_res = getaddrinfo(HTTP_parser::get_atribute("host").toStdString().c_str(),"80", &hints, &res);
    web_sock_fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    connect(web_sock_fd,res->ai_addr,res->ai_addrlen);
    send(web_sock_fd,buffer,1<<15,0);
    return web_sock_fd;
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


    char buffer[1 << 15];
    while(1) {
        int browser_socket;
        if ((browser_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

//        read(browser_socket, buffer, 1<<15);
        read_until_terminators(browser_socket, buffer, BRBN, 4);
        debug("received browser request");
        //QString request(buffer);
        QString request(buffer);
        qDebug() << request << endl;
        int web_sock_fd = send_request_to_the_web(buffer);
        debug("sent request to the web");
        //receives response
        read(web_sock_fd, buffer, (1<<15));

//        int ammount_read = read_until_terminators(web_sock_fd, buffer, BRBN, 4);
//        debug("read browser response");
        QString response_header(buffer);
        qDebug() << response_header << endl;
//        HTTP_parser::parse(response_header);
//        int cont_length = HTTP_parser::get_atribute("content-length").toInt();
//        read(web_sock_fd,buffer + ammount_read, cont_length);
        //sends it back to the browser
//        write(browser_socket, buffer, ammount_read + cont_length);
        write(browser_socket, buffer, 1 << 15);
        debug("wrote to browser");
        close(web_sock_fd);
        close(browser_socket);
    }
}

