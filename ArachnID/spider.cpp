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

QString Spider::build_request_for_path(QString host, QString path) {
    QString first_line = "GET http://" + host + path + " HTTP/1.1";
    map<QString, QString> fields;
    fields["host"] = host;
    fields["connection"] = "close";
    fields["accept-encoding"] = "identity";
    return HTTP_Helper::build_html_header(fields, first_line);
}

QStringList Spider::extract_links(QString content) {
    int next = 0;
    QStringList result;
    QString pattern = "<a href=\"";
    while(int next_index = content.indexOf(pattern, next) != -1) {
//        qDebug() << "lopp extract\n";
        int link_start = next_index + pattern.size();
        QString acc;
        int i = link_start;
        while(content[i] != '\"') {
//            qDebug() << "sublopp extract\n";
            acc.append(content[i]);
            i++;
        }
        next = link_start + 1;
        result.append(acc);
    }
    return result;
}

map<QString, vector<QString>> Spider::crawl_page(QString host, QString start_path) {
    map<QString, vector<QString>> graph;
    //          path     file
    queue<QString> q;
    char buf[1 << 22];
    char BRBN[4] = {'\r', '\n', '\r', '\n'};
    set<QString> enqueued;
    q.push(start_path);
    enqueued.emplace(start_path);
    while(q.size()) {
        QString cur = q.front();
        q.pop();
        qDebug() << cur << endl;
        QString request = build_request_for_path(host, cur);
        qDebug() << request << endl;
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

        QStringList links = extract_links(content);

        for(QString link : links) {
            qDebug() << "link: " << link << endl;
            // link vazio, nao deveria acontecer em páginas normais,
            // mas é possível sair de output da função.
            if(link.isEmpty()) continue;
            // link externo
            if(link.startsWith("http")) continue;

            // URL absoluta
            if(link.startsWith("/")) {
//                QString actual_link = link.right(int(link.size()) - 1);
                if(enqueued.find(link) != enqueued.end()) continue;
                enqueued.emplace(link);
                graph[cur].emplace_back(link);
                q.push(link);
                continue;
            }

            // a partir daqui o link é (ou deveria ser) relativo
            QString cur_path = cur.left(cur.lastIndexOf("/") + 1);
            QString cur_file = cur.right(int(cur.size() - cur.lastIndexOf("/") - 1));
            QString actual_link;
            if(link.startsWith("./")) {
                actual_link = cur_path + link.right(int(link.size()) - 2);
            } else {
                actual_link = cur_path + link;
            }
            if(enqueued.find(actual_link) != enqueued.end()) continue;
            enqueued.emplace(cur_path + actual_link);
            q.push(cur_path + actual_link);
            graph[cur].emplace_back(cur_path + actual_link);
        }

    }
    qDebug() << "finished crawling\n";
    return graph;
}
