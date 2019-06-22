#include <spider.h>
#include <QString>
#include <bits/stdc++.h>
#include "http_helper.h"
using namespace std;

//QString CRNL{QChar::CarriageReturn, QChar::LineFeed};


map<QString, vector<QString>> crawl_page(QString host, QString start_path) {
    //          path     file
    queue<QString> q;
    q.push(start_path);
    while(q.size()) {
        QString cur = q.front();
        q.pop();
        QString first_line = "GET " + host + start_path + " HTTP/1.1";
        map<QString, QString> fields;
        fields[QString("host")] = host;
        fields[QString("connection")] = QString("close");
//        HTTP_Helper::build_html_header(fields, first_line);
    }
    return {};
//    return {};
}
