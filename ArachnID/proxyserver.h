#ifndef PROXYSERVER_H
#define PROXYSERVER_H

#include <QObject>
#include <QDebug>
#include <QThread>
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


class ProxyServer : public QObject
{
    Q_OBJECT
public:
    explicit ProxyServer(QObject *parent = nullptr);
    ~ProxyServer();

    void setup();
signals:
    void got_request(QString request);
    void got_response(QString response_header);


public slots:
    void stop();
    void send_request_to_the_web(QString request);
    void send_response_to_the_browser(QString response);

private:
    bool mStop = false;
    char BRBN[4] = {'\r', '\n', '\r', '\n'};
    int create_server_fd();
    void attach_socket_to_port(int server_fd,struct sockaddr_in *address, int port = 8080);
    void debug(string debug);
    struct sockaddr_in address;
    void listen_browser();
    int addrlen;
    int server_fd;
    int browser_socket;
    char buffer[1 << 22];
    int web_sock_fd;
    bool isText = true;
    int content_length;

};


#endif // PROXYSERVER_H
