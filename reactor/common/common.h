/*************************************************************************
	> File Name: common.h
	> Author: wushanning 
	> Mail: 954726270@qq.com 
	> Created Time: Wed Mar 24 21:17:53 2021
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

int make_nonblock(int fd);
int make_block(int fd);
int socket_create(int port);
int socket_connect(const char *ip, int port);

#endif
