//
//  main.c
//  daytimetcpsrv
//
//  Created by 常峻玮 on 16/3/2.
//  Copyright © 2016年 Mingle. All rights reserved.
//

#include	"unp.h"
#include	<time.h>

int Socket(int family,int type,int protocol){
    int n;
    if ((n=socket(family, type, protocol))<0) {
        err_sys("socket error");
    }
    return n;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if (bind(fd, sa, salen) < 0)
        err_sys("bind error");
}



int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    int  n;
    
again:
    if ( (n = accept(fd, sa, salenptr)) < 0) {
#ifdef EPROTO
        if (errno == EPROTO || errno == ECONNABORTED)
#else
        if (errno == ECONNABORTED)
#endif
            goto again;
        else
            err_sys("accept error");
    }
    return(n);
}

void Listen(int fd, int backlog)
{
    char *ptr;
    
    /*4can override 2nd argument with environment variable */
    if ( (ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);
    
    if (listen(fd, backlog) < 0)
        err_sys("listen error");
}

#pragma mark - Main
int main(int argc, char **argv)
{
    int					listenfd, connfd;
    struct sockaddr_in	servaddr;
    char				buff[MAXLINE];
    time_t				ticks;
    
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(9090);	/* daytime server */
    
    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
    
    Listen(listenfd, LISTENQ);
    
    for ( ; ; ) {
        connfd = Accept(listenfd, (SA *) NULL, NULL);
        
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
//        write(connfd, buff, strlen(buff));
        for (int i=0; i<strlen(buff); i++) {
            write(connfd, &buff[i], 1);
        }
        close(connfd);
    }
}

