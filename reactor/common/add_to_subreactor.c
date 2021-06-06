/*************************************************************************
	> File Name: add_to_subreactor.c
	> Author: wushanning 
	> Mail: 954726270@qq.com 
	> Created Time: Wed Jun  2 16:06:32 2021
 ************************************************************************/

#include "head.h"

int add_event_ptr (int subepollfd, struct User *user) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = (void *)user;
    if (epoll_ctl(subepollfd, EPOLL_CTL_ADD, user->fd, &ev) < 0) {
        return -1;
    }
    return 0;
}

int add_to_subreactor(struct User *user) {
    // 前置已经完成了数据的复制与保存
    if (add_event_ptr(user->subepollfd, user) < 0) {
        return -1;
    }
    printf("Add <User %s> to subreactor!\n", user->name);
    return 0;
}
