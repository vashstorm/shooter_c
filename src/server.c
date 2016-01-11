/*  **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : server.c
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-04 14:21:48
 *  Version : 0.01
 ************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <jemalloc/jemalloc.h>
#include <sys/eventfd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/epoll.h>
#include "internal.h"
#include "queue.h"
#include "worker.h"
#include "server.h"

static int l_thread_num = 8;
static int l_compress   = 1;
static int l_listenfd   = 0;

static int MAX_EV = 32;

static shooter_callback l_cb;

// server_info > 0, host is ip
// server_info = 0, host is unix socket
int  shooter_init(const char* host, int server_info, shooter_callback cb) {
    if (!cb) {
        printf("callback is NULL\n");
        return -1;
    }

    l_cb = cb;
    int  on = 1;

    if (server_info > 0) {
        // tcp 
        int             n;
        struct addrinfo hints, *res, *ressave;
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_flags = AI_PASSIVE;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        char serv[32] = {0};
        sprintf(serv, "%d", server_info);
        if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
            printf("tcp_listen error for %s, %s: %s", host, serv, gai_strerror(n));            
        ressave = res;                                        
        do {                                                                       
            l_listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); 
            if (l_listenfd < 0)
                continue;

            setsockopt(l_listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));       
            if (bind(l_listenfd, res->ai_addr, res->ai_addrlen) == 0) {
                TRACE("[%s] [%d]\n", res->ai_addr, res->ai_addrlen);
                break;
            }

            close(l_listenfd);
            l_listenfd = 0;
        }                                                                          
        while ((res = res->ai_next) != NULL);

        if (res == NULL)
            printf("tcp_listen error for %s, %s", host, serv); 

        TRACE("res is not null\n");

        if (listen(l_listenfd, 1024)) {
            perror("");
            return 0;
        }
        // if (addrlenp)                                                             
        //     *addrlenp = res->ai_addrlen;

        freeaddrinfo(ressave);

        return (l_listenfd);
    }
    else {
        // unix socket
    }

    return 0;
}

void shooter_run() {
    int i = 0;
    // accept fd is store here
    SH_QUEUE fdarr[l_thread_num];
    // eventfd 
    int    evarr[l_thread_num];

    for (i = 0; i < l_thread_num; ++i) {
        shq_init(&fdarr[i]);
        evarr[i] = -1;
    }
    
    TRACE("thread number [%d]\n", l_thread_num);
    // start thread pool
    for (i = 0; i < l_thread_num; ++i) {
        pthread_t  pid;
        int ev = eventfd(0, EFD_NONBLOCK);
        evarr[i] = ev;
        EVCB* evcb = calloc(1, sizeof(EVCB));
        evcb->ev = ev;
        evcb->cb = l_cb;
        if (pthread_create(&pid, NULL, shooter_worker, (void*)evcb)) {
            perror("");
            return;
        } 
        else {
            TRACE("pid [%lu]\n", pid);
        }
        pthread_detach(pid);
    }

    TRACE("create thread over\n");

    int epfd = epoll_create(1024);
    struct epoll_event  ev;
    struct epoll_event  allev[MAX_EV];
    ev.data.fd = l_listenfd;
    ev.events  = EPOLLIN|EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);

    int index = 0;
    for (;;) {
        int cnt = epoll_wait(epfd, allev, MAX_EV, 10000);

        if (!cnt) {
            continue;
        }
        
        int i = 0;

        for (; i < cnt; ++i) {
            if (allev[i].data.fd == l_listenfd) {   // new connect
                TRACE("listenfd is readable!\n");
                uint64_t connfd = accept(l_listenfd, NULL, NULL); 
                TRACE("newconn  is [%d], evarr[%d] [%d]\n", connfd, index, evarr[index]);
                // stored connfd into fdarr[index]
                // and epoll add writing event for eventfd[index]
                shq_push(&fdarr[index], connfd);
                ev.data.u32 = index;       // 
                ev.events   = EPOLLOUT|EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_ADD, evarr[index], &ev);
                ++index;
                index %= l_thread_num;
                continue;
            }
            
            if (allev[i].events & EPOLLOUT) { 
                int handleindex = allev[i].data.u32;
                int newfd = 0;
                int ret = shq_pop(&fdarr[handleindex], &newfd);
                uint64_t hh = newfd;
                TRACE("server EPOLLOUT fd[%d], eventfd [%d]\n", newfd, evarr[handleindex]);
                write(evarr[handleindex], &hh, sizeof(hh));
                // not fd to handle
                if (0 == ret) {
                    TRACE("ret is 0 now!\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, evarr[index], NULL);
                }
            }
        }
    }
}

// name : THREAD_NUM 
void shooter_set(const char* name, int val) {
    if (!strcasecmp("thread_num", name)) {
        l_thread_num = val;
    }
    else if (!strcasecmp("compress", name)) {
        l_compress = val;
    }
}

