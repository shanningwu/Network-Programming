#!/bin/bash
gcc 1.server.c common/common.c common/thread_pool.c common/add_to_subreactor.c -l pthread -I common/ -o server
gcc 2.client.c common/common.c -I common/ -o client
