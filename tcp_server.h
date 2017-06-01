#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#define MYPORT  8887
#define QUEUE   20

bool tcp_server_init(int &server_sockfd, int &conn) {
    ///定义sockfd
    server_sockfd = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ///bind，成功返回0，出错返回-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1) {
        perror("bind");
        return 0;
    }

    ///listen，成功返回0，出错返回-1
    if(listen(server_sockfd,QUEUE) == -1) {
        perror("listen");
        return 0;
    }

    ///客户端套接字
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    ///成功返回非负描述字，出错返回-1
    conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    if(conn<0) {
        perror("connect");
        return 0;
    }
    //printf("before_conn\n");
    return 1;
}

bool tcp_server_close(int &server_sockfd, int &conn) {
    close(conn);
    close(server_sockfd);
    return 1;
}
