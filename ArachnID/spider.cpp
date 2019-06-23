#include <spider.h>
#include <QString>
#include <bits/stdc++.h>
#include "http_helper.h"
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

using namespace std;

//QString CRNL{QChar::CarriageReturn, QChar::LineFeed};

QString build_request_for_path(QString host, QString path) {
    QString first_line = "GET " + host + path + " HTTP/1.1";
    map<QString, QString> fields;
    fields["host"] = host;
    fields["connection"] = "close";
    fields["accept-encoding"] = "identity";
    return HTTP_Helper::build_html_header(fields, first_line);
}

int connect_and_get_socket(QString host) {
    struct addrinfo hints, *res;
    memset(&hints, 0,sizeof hints);
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    int get_addr_res = getaddrinfo(host.toStdString().c_str(),"80", &hints, &res);
    if(get_addr_res == EAI_NONAME) {
        qDebug() << "Esse host não foi encontrado!" << endl;
//        QtConcurrent::run(this, &ProxyServer::listen_browser);
        exit(1);
    }
    int web_sock_fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    ::connect(web_sock_fd,res->ai_addr,res->ai_addrlen);
    return web_sock_fd;
}


map<QString, vector<QString>> crawl_page(QString host, QString start_path) {
    map<QString, vector<QString>> graph;
    //          path     file
    queue<QString> q;
    q.push(start_path);
    while(q.size()) {
        QString cur = q.front();
        q.pop();
        QString request = build_request_for_path(host, cur);
        int web_socket = connect_and_get_socket(host);
        write(web_socket, request.toStdString().c_str(), request.toStdString().size());
    }
    return graph;
//    return {};
}
