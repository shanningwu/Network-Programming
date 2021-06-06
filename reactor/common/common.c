/*************************************************************************
	> File Name: common.c
	> Author: wushanning 
	> Mail: 954726270@qq.com 
	> Created Time: Wed Mar 24 21:19:33 2021
 ************************************************************************/

#include "head.h"

int make_nonblock(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        return -1;
    }
    flag |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flag) < 0) {
        return -1;
    }
    return 0;
}

int make_block(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        return -1;
    }
    flag &= ~O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flag) < 0) {
        return -1;
    }
    return 0;
}

int socket_create(int port) {
    int sockfd;
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        return -1;
    }
    if (listen(sockfd, 20) < 0) {
        return -1;
    }
    return sockfd;
}

int socket_connect(const char *ip, int port) {
    int sockfd;
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        return -1;
    }
    return sockfd;
}
