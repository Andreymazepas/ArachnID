#include "proxyserver.h"
#include "common.h"
#include <QtConcurrent>
#include "socket_utils.h"

ProxyServer::ProxyServer(QObject *parent) : QObject(parent) {}

ProxyServer::~ProxyServer(){
    mStop = false;
}

void ProxyServer::setup( ) {
    server_fd = SocketUtils::create_server_socket(&address);
    listen_browser();
}
void ProxyServer::listen_browser() {
    if ((browser_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    int amm_read  = SocketUtils::read_until_terminators(browser_socket, buffer, BRBN, 4);
    buffer[amm_read] = 0;
    QString request(buffer);
    emit got_request(request);
}

string fix_lost_characters(string payload) {
    string result = "";
    for(auto c : payload) {
        if(c != '\n') {
            result += c;
        } else {
            result += '\r';
            result += '\n';
        }
    }
    return result;
}


void ProxyServer::send_request_to_the_web(QString request) {
    string fixed = fix_lost_characters(request.toStdString());
    QString first_line;
    map<QString, QString> fields;
    tie(fields, first_line) = HTTP_Helper::parse_html_header(QString(fixed.c_str()));
    fields = HTTP_Helper::simplify_http_header(fields);
    fixed = HTTP_Helper::build_html_header(fields, first_line).toStdString();
    web_sock_fd = SocketUtils::connect_and_get_socket(fields["host"]);
    send(web_sock_fd,fixed.c_str(),fixed.size(),0);
    int amm_read = SocketUtils::read_until_terminators(web_sock_fd, buffer, BRBN, 4);
    buffer[amm_read] = 0;
    string aux(buffer);
    QString response_header(buffer);
    tie(fields, first_line) = HTTP_Helper::parse_html_header(response_header);
    isText = fields["content-type"].contains("text");
    content_length = fields["content-length"].toInt();
    amm_read = 0;
    while(amm_read < content_length) {
        amm_read += read(web_sock_fd, buffer+amm_read, 1 << 15);
    }
    buffer[amm_read] = 0;
    QString response_body(buffer);
    emit got_response(response_header + (isText ? response_body : ""));
}

pair<QString, QString> split_header_and_body(QString whole_response) {
    int first_occ = whole_response.indexOf("\n\n");
    if(first_occ == -1) {
        qDebug() << "expected header to contain \n\n but nope" << endl;
        exit(0);
    }
    QString header = "", body = "";
    for (int i = 0; i < first_occ; i++) {
        header.append(whole_response[i]);
    }
    header.append("\n\n");

    for(int i = first_occ + 2; i < whole_response.size(); i++) {
        body.append(whole_response[i]);
    }
    return {header, body};
}

void ProxyServer::send_response_to_the_browser(QString whole_response) {
    QString header, body;
    string Cresponse;
    if(isText) {
        tie(header, body) = split_header_and_body(whole_response);
        Cresponse = fix_lost_characters(header.toStdString()) + body.toStdString();
        write(browser_socket, Cresponse.c_str(), Cresponse.size());
    } else {
        header = whole_response;
        Cresponse = fix_lost_characters(header.toStdString());
        write(browser_socket, Cresponse.c_str(), Cresponse.size());
        write(browser_socket, buffer, content_length);
    }
    close(web_sock_fd);
    close(browser_socket);
    QFuture<void> _ = QtConcurrent::run(this, &ProxyServer::listen_browser);
}

void ProxyServer::stop() {
    mStop = true;
}

void ProxyServer::debug(string s) {
    qDebug() << QString(s.c_str()) << endl;
}

