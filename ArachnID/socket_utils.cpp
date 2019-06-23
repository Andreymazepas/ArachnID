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
