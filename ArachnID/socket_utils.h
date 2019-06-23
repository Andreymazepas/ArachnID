#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H


class SocketUtils {
public:
    static int read_until_terminators(int sock_fd,char* buf, char* terminator, int terminators_size);
};

#endif // SOCKET_UTILS_H
