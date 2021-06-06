/*************************************************************************
	> File Name: 1.server.c
	> Author: wushanning 
	> Mail: 954726270@qq.com 
	> Created Time: Wed Jun  2 15:16:58 2021
 ************************************************************************/

#include "head.h"
#define MAX 100
#define MAXQUEUE 10
#define EPOLL_SIZE 5

void *do_work(void *arg) {
    int subepollfd = *(int *)arg;
    struct epoll_event events[5];
    while (1) {
        int nfds = epoll_wait(subepollfd, events, EPOLL_SIZE, -1);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(1);
        }
        for (int i = 0; i < nfds; ++i) {
            char buff[512] = {0};
            struct User *user;
            user = (struct User *)events[i].data.ptr;
            int nrecv = recv(user->fd, buff, sizeof(buff), 0);
            if (nrecv < 0) {
                epoll_ctl(subepollfd, EPOLL_CTL_DEL, user->fd, NULL);
                close(user->fd);
                user->online = 0;
            }
            printf("Msg from <subreactor%d-User %s> : %s\n", subepollfd, user->name, buff);
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    int opt, port, listener, epollfd, epollfd_boy, epollfd_girl;
    struct User *users_girl, *users_boy;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage : %s -p port\n", argv[0]);
                exit(1);
        }
    }
    if ((listener = socket_create(port)) < 0) {
        perror("socket_create");
        exit(1);
    }
    // 创建一主两从反应堆
    epollfd = epoll_create(1);
    epollfd_boy = epoll_create(1);
    epollfd_girl = epoll_create(1);
    if (epollfd < 0 || epollfd_boy < 0 || epollfd_girl < 0) {
        perror("epoll_create");
        exit(1);
    } 
    // 分配两个用户空间
    users_boy = (struct User *)calloc(MAX, sizeof(struct User));
    users_girl = (struct User *)calloc(MAX, sizeof(struct User));
    // 初始化两个任务队列，用线程池，待优化
    // struct task_queue boyQueue;
    // struct task_queue girlQueue;
    // task_queue_init(&boyQueue, MAXQUEUE);
    // task_queue_init(&girlQueue, MAXQUEUE);
    pthread_t tid_boy, tid_girl;
    pthread_create(&tid_boy, NULL, do_work, &epollfd_boy);
    pthread_create(&tid_girl, NULL, do_work, &epollfd_girl);
    printf("<%d> : boy\n<%d> : girl\n", epollfd_boy, epollfd_girl);
    // 初始化主反应堆
    struct epoll_event ev, events[EPOLL_SIZE];
    ev.events = EPOLLIN;
    ev.data.fd = listener;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listener, &ev) < 0) {
        perror("epoll_ctl");
        exit(1);
    } 
    for (;;) { 
        int nfds = epoll_wait(epollfd, events, EPOLL_SIZE, -1);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(1);
        } 
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == listener) {
                struct sockaddr_in client;
                int new_fd;
                struct logRequest request;
                struct logResponse response;
                socklen_t len = sizeof(client);
                if ((new_fd = accept(listener, (struct sockaddr *)&client, &len)) < 0) {
                    perror("accept");
                    exit(1);
                }
                recv(new_fd, (void *)&request, sizeof(request), 0); // 阻塞等待用户信息
                printf("ip = %s, client = %s\n", inet_ntoa(client.sin_addr), request.name);
                // 验证用户信息, 验证会员状态等等操作
                // 填充, 保留用户信息
                struct User *tmp;
                if (request.sex) {
                    tmp = users_boy;
                    tmp[new_fd].subepollfd = epollfd_boy;
                } else { 
                    tmp = users_girl;
                    tmp[new_fd].subepollfd = epollfd_girl;
                }
                strcpy(tmp[new_fd].name, request.name);
                strcpy(tmp[new_fd].ip, inet_ntoa(client.sin_addr));
                tmp[new_fd].online = 1;
                tmp[new_fd].fd = new_fd;
                response.flag = 1;
                strcpy(response.msg, "OK, connected!");
                send(new_fd, (void *)&response, sizeof(response), 0);
                if (add_to_subreactor(&tmp[new_fd]) < -1) {
                    perror("add_to_subreactor");
                    exit(1);
                }
            }
        }
    }
    return 0;
}
