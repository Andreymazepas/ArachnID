#include "proxyserver.h"
#include "common.h"
#include <QtConcurrent>

ProxyServer::ProxyServer(QObject *parent) : QObject(parent) {}

ProxyServer::~ProxyServer(){
    mStop = false;
}

void ProxyServer::setup( ) {

    addrlen = sizeof(address);
    server_fd = create_server_fd();
    attach_socket_to_port(server_fd, &address);
    if (listen(server_fd, 25) < 0)  {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    listen_browser();
}
void ProxyServer::listen_browser() {
    debug("hmmmm\n");
    if ((browser_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    debug("escutando!!!\n");
//    read(browser_socket, buffer, 1 << 1                                                                                                                                                                                   5);
    int amm_read  = read_until_terminators(browser_socket, buffer, BRBN, 4);
    buffer[amm_read] = 0;
    debug("received browser request");
    //QString request(buffer);
    QString request(buffer);
    qDebug() << request << endl;
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
    struct addrinfo hints, *res;
    debug(request.toStdString());
    string fixed = fix_lost_characters(request.toStdString());

    //get host info, make socket and connect it
    memset(&hints, 0,sizeof hints);
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    debug("will parse!");
    debug(fixed);

    QString first_line;
    map<QString, QString> fields;

    tie(fields, first_line) = HTTP_Helper::parse_html_header(QString(fixed.c_str()));
    qDebug() << first_line << endl;
    debug("parsed");
    debug("host is + " + fields["host"].toStdString());

    int get_addr_res = getaddrinfo(fields["host"].toStdString().c_str(),"80", &hints, &res);
    if(get_addr_res == EAI_NONAME) {
        qDebug() << "Esse host não foi encontrado!" << endl;
        QtConcurrent::run(this, &ProxyServer::listen_browser);
        return ;
    }
    debug("got address");

    fields = HTTP_Helper::simplify_http_header(fields);

    fixed = HTTP_Helper::build_html_header(fields, first_line).toStdString();
    debug(fixed);
    web_sock_fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    debug("made socket");
    ::connect(web_sock_fd,res->ai_addr,res->ai_addrlen);
    debug("connected");

    debug("will send request");
    send(web_sock_fd,fixed.c_str(),fixed.size(),0);
    debug("sent request");
    int amm_read = read_until_terminators(web_sock_fd, buffer, BRBN, 4);
    buffer[amm_read] = 0;
//    read(web_sock_fd, buffer, (1<<15));
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
    debug(aux);
    qDebug() << response_header << endl;
    qDebug() << response_body << endl;
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
    debug("sending to browser");
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
    qDebug() << header<< endl;
    qDebug() << body << endl;

    debug(Cresponse);

    debug("wrote");
    close(web_sock_fd);
    close(browser_socket);
    QFuture<void> _ = QtConcurrent::run(this, &ProxyServer::listen_browser);

}

void ProxyServer::stop() {
    mStop = true;
}

int ProxyServer::create_server_fd() {
    int server_fd;
    debug("create_server_fd");
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)  {
        debug("socket failed");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

void ProxyServer::attach_socket_to_port(int server_fd,struct sockaddr_in *address, int port) {
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
    printf("%d, %d\n", server_fd, port);
    qDebug() << server_fd << " " << port;
}

void ProxyServer::debug(string s) {
    qDebug() << QString(s.c_str()) << endl;
}

int ProxyServer::read_until_terminators(int sock_fd,char* buf, char* terminator, int terminators_size) {
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
        if(total == 1 << 15){
            qDebug() << "dios mio" << endl;
            buf[1 << 15] = 0;
            qDebug() << QString(buf) << endl;
        }
    }
    return total;
}
