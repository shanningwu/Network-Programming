/*************************************************************************
	> File Name: 2.client.c
	> Author: wushanning 
	> Mail: 954726270@qq.com 
	> Created Time: Wed Jun  2 16:46:25 2021
 ************************************************************************/

#include "head.h"

int main(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "Usage : %s ip port name sex[0 for girl, 1 for boy]!\n", argv[0]);
        exit(1);
    }
    int sockfd;
    if ((sockfd = socket_connect(argv[1], atoi(argv[2]))) < 0) {
        perror("socket_connect");
        exit(1);
    }
    struct logRequest request;
    struct logResponse response;
    memset(&request, 0, sizeof(request));
    memset(&response, 0, sizeof(response));
    strcpy(request.name, argv[3]);
    request.sex = atoi(argv[4]);
    send(sockfd, (void *)&request, sizeof(request), 0); // 发送用户信息
    recv(sockfd, (void *)&response, sizeof(response), 0); // 接收服务端完成注册应答
    printf("flag = %d, msg = %s\n", response.flag, response.msg);
    while (1) {
        char buff[512] = {0};
        scanf("%[^\n]s", buff);
        getchar();
        if (strlen(buff)) {
            send(sockfd, buff, strlen(buff), 0);
        }
    }
    return 0;
}
