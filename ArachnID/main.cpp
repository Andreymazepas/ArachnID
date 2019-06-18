#include "mainwindow.h"
#include <QApplication>
#include "pthread.h"
#include "server.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define MSQID 140140522

pthread_t server_thread;
long queueId[1];

long obtainQueue() {
    struct msqid_ds buf;
    long aux;
    if((aux = msgget(MSQID, IPC_CREAT | 0x1B6)) < 0) {
        printf("erro na criacao da fila %ld - errno: %d \n", aux, errno);
        exit(1);
    }

    if(msgctl(int(aux), IPC_RMID, &buf) == -1) {
        printf("Erro ao deletar p fila de mensagens\n");
    }

    if((aux = msgget(MSQID, IPC_CREAT|0x1B6|IPC_EXCL)) < 0) {
        printf("erro na criacao da fila %ld - errno: %d \n", aux, errno);
        exit(1);
    }
    return aux;
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //queueId[0] = obtainQueue();
    int val;
    //pthread_create(&server_thread, NULL, server, (void*) queueId);
    auto ret = a.exec();
    //pthread_cancel(server_thread);
    return ret;
}

