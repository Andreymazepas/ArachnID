#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H
#include <QString>

class SocketUtils {
public:
    static int read_until_terminators(int sock_fd,char* buf, char* terminator, int terminators_size);
    static int connect_and_get_socket(QString host);
    static int create_server_socket(struct sockaddr_in* address, int port = 8080);
    static void read_exactly(int sock_fd, char* buf, int num_bytes);
};

#endif // SOCKET_UTILS_H
