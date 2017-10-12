//
//  main.c
//  tcpcli01
//
//  Created by 常峻玮 on 17/1/1.
//  Copyright © 2017年 Mingle. All rights reserved.
//

#include <stdio.h>
#include "unp.h"

//void str_cli(FILE *fp, int sockfd) {
//    char sendline[MAXLINE], recvline[MAXLINE];
//    while (Fgets(sendline, MAXLINE, fp) != NULL) {
//        Write(sockfd, sendline, strlen(sendline));
//        if (Readline(sockfd, recvline, MAXLINE) == 0) {
//            err_quit("str_cli: server terminated prematurely");
//        }
//        Fputs(recvline, stdout);
//    }
//}

int main(int argc, const char * argv[]) {
    // insert code here...
    
    int sockfd;
    struct sockaddr_in servaddr;
    
    if (argc != 2) {
        err_quit("usage: tcpcli <IPaddress>");
    }
    
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    
    Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
    
    str_cli(stdin, sockfd);
    
    return 0;
}
