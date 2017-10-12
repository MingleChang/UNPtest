//
//  main.c
//  tcpserv01
//
//  Created by 常峻玮 on 17/1/1.
//  Copyright © 2017年 Mingle. All rights reserved.
//

#include <stdio.h>
#include "unp.h"

void str_echo(int sockfd) {
    ssize_t n;
    char buf[MAXLINE];
again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0 ) {
        Write(sockfd, buf, n);
    }
    if (n < 0 && errno == EINTR) {
        goto again;
    }else if (n < 0) {
        err_sys("str_echo: read error");
    }
}
int AcceptNew(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    int  n;
    
again:
    if ( (n = accept(fd, sa, salenptr)) < 0) {
#ifdef EPROTO
        if (errno == EPROTO || errno == ECONNABORTED || errno == EINTR)
#else
            if (errno == ECONNABORTED)
#endif
                goto again;
            else
                err_sys("accept error");
    }
    return(n);
}
int main(int argc, const char * argv[]) {
    // insert code here...
    
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    
    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    
    Listen(listenfd, LISTENQ);
    
    for (; ; ) {
        clilen = sizeof(cliaddr);
        connfd = AcceptNew(listenfd, (SA *)&cliaddr, &clilen);
        if ((childpid = Fork()) == 0) {
            Close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        Close(connfd);
    }
    
    return 0;
}
