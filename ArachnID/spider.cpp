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
#include "socket_utils.h"

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

map<QString, vector<QString>> crawl_page(QString host, QString start_path) {
    map<QString, vector<QString>> graph;
    //          path     file
    queue<QString> q;
    char buf[1 << 22];
    char BRBN[4] = {'\r', '\n', '\r', '\n'};
    q.push(start_path);
    while(q.size()) {
        QString cur = q.front();
        q.pop();
        QString request = build_request_for_path(host, cur);
        int web_socket = SocketUtils::connect_and_get_socket(host);

        write(web_socket, request.toStdString().c_str(), request.toStdString().size());

        int header_size = SocketUtils::read_until_terminators(web_socket, buf, BRBN, 4);
        buf[header_size] = 0;
        QString response_header(buf);

        map<QString, QString> fields;
        QString first_line;
        tie(fields, first_line) = HTTP_Helper::parse_html_header(response_header);
        SocketUtils::read_exactly(web_socket, buf, fields["content-length"].toInt());
        QString content(buf);

        qDebug() << response_header << endl;
        qDebug() << content << endl;

    }
    return graph;
//    return {};
}
