/*************************************************************************
	> File Name: head.h
	> Author: wushanning 
	> Mail: 954726270@qq.com 
	> Created Time: Fri 19 Mar 2021 06:37:47 PM CST
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#include "common.h"
#include "my_popen.h"
#include "thread_pool.h"
#include "reactor.h" // 头文件顺序, 得先于 add_to_subreactor.h, 否则类型找不到出错
#include "add_to_subreactor.h"

#endif
