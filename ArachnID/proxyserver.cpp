#include "proxyserver.h"
#include "common.h"

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
//    read(browser_socket, buffer, 1 << 15);
    read_until_terminators(browser_socket, buffer, BRBN, 4);
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
    auto options = HTTP_Helper::parse_html_header(QString(fixed.c_str()));
    debug("parsed");
    debug("host is + " + options["host"].toStdString());
    int get_addr_res = getaddrinfo(options["host"].toStdString().c_str(),"80", &hints, &res);
    debug("got address");

    web_sock_fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    debug("made socket");
    ::connect(web_sock_fd,res->ai_addr,res->ai_addrlen);
    debug("connected");
    debug("will send request");
    send(web_sock_fd,fixed.c_str(),1<<15,0);
    debug("sent request");
    read(web_sock_fd, buffer, (1<<15));
    QString response_header(buffer);
    qDebug() << response_header << endl;
    emit got_response(response_header);
}

void ProxyServer::send_response_to_the_browser(QString response) {
    string Cresponse = fix_lost_characters(response.toStdString());
    write(browser_socket, Cresponse.c_str(), 1<<15);
    close(web_sock_fd);
    close(browser_socket);
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
    }
    return total;
}
