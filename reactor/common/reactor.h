/*************************************************************************
	> File Name: reactor.h
	> Author: wushanning 
	> Mail: 954726270@qq.com 
	> Created Time: Wed Jun  2 16:09:14 2021
 ************************************************************************/

#ifndef _REACTOR_H
#define _REACTOR_H

struct User {
    int fd;
    char name[20];
    char ip[20];
    int online;
    int subepollfd;
};

struct logRequest {
    char name[20];
    int sex; // 0 girl, 1 boy
};

struct logResponse {
    int flag; // 0 false, 1 true
    char msg[512];
};

#endif
