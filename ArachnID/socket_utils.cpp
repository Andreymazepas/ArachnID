#include "socket_utils.h"
#include "proxyserver.h"


int SocketUtils::read_until_terminators(int sock_fd,char* buf, char* terminator, int terminators_size) {
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
        if(total == 1 << 15) {
            qDebug() << "dios mio" << endl;
            buf[1 << 15] = 0;
            qDebug() << QString(buf) << endl;
        }
    }
    return total;
}

int SocketUtils::connect_and_get_socket(QString host) {
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

int SocketUtils::create_server_socket(struct sockaddr_in* address, int port) {
    int addrlen = sizeof(address);

    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)  {
        exit(EXIT_FAILURE);
    }
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
    if (listen(server_fd, 25) < 0)  {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

void SocketUtils::read_exactly(int sock_fd, char* buf, int num_bytes) {
    int amm_read = 0;
    while(amm_read < num_bytes) {
        amm_read += read(sock_fd, buf + amm_read, 1 << 15);
    }
}
