/* **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : worker.c
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-05 10:46:17
 *  Version : 0.01
 * **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <jemalloc/jemalloc.h>
#include "internal.h"
#include "hashtable.h"
#include "package.h"

#define MAX_EV 32

void* shooter_worker(void* arg) {
    int              event  = ((EVCB*)arg)->ev;
    shooter_callback pcb    = ((EVCB*)arg)->cb; 
    free(arg);


    // hashtable
    HashTable ht;
    HT_create(&ht);

    // work work ~~
    struct epoll_event ev;
    struct epoll_event hpev[MAX_EV];
    int epfd = epoll_create(1024);

    TRACE("thread start [%lu], eventfd [%d], epollfd [%d]\n", pthread_self(), event, epfd);

    ev.data.fd = event;
    ev.events  = EPOLLIN|EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, event, &ev);

    for (;;) {
        int cnt = epoll_wait(epfd, hpev, MAX_EV, 3000); 
        
        if (!cnt) {
            continue;
        }
        
        int i = 0;
        char buffer[1024*1024];
        char cache[1024*1024];

        for (; i < cnt; ++i) {
            // add newfd, event only has epollout event
            if (hpev[i].data.fd == event) {
                uint64_t hh;
                read(hpev[i].data.fd, &hh, sizeof(hh));
                int newfd = hh;
                ev.data.fd  = newfd;
                ev.events   = EPOLLIN|EPOLLET;
                TRACE("worker add connect [%d]\n", newfd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, newfd, &ev);
                continue;
            }
            
            if (hpev[i].events & EPOLLIN) {         // read
                // nn is not null
                HashNode* nn = HT_getx(&ht, hpev[i].data.fd);
                int n = read(hpev[i].data.fd, buffer, sizeof(buffer));
                if (n <= 0) {  // close 
                    TRACE("close [%d]\n", hpev[i].data.fd);
                    close(hpev[i].data.fd);
                    HT_del(&ht, hpev[i].data.fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, hpev[i].data.fd, NULL);
                    continue;
                }

                int len = 0;
                if (nn->data == NULL) {      // donot need join 
                    int ret = shooter_check_complete_line(buffer, n);
                    if (0 == ret) { // not complete, put msg into hashtable
                        TRACE("nn->data is NULL and check return 0 [%s]\n", nn->data);
                        nn->data = (char*)malloc(n);
                        nn->len  = n;
                        memcpy(nn->data, buffer, n);
                    }
                    else if (ret > 0) {
                        TRACE("nn->data in NULL and check return %d\n", ret);
                        if (ret < n) {      // next msg is here~
                            TRACE("----------- ZB ret [%d] n [%d]\n", ret, n);
                            nn->data = (char*)malloc(n - ret);
                            memcpy(nn->data, buffer+ret, n-ret);
                        }

                        len = pcb(buffer, ret, cache, sizeof(cache));
                    }
                }
                else {  // need join msg
                    nn->data = realloc(nn->data, nn->len+n);
                    memcpy(nn->data+nn->len, buffer, n);
                    nn->len += n;
                    int ret = shooter_check_complete_line(nn->data, nn->len);
                    TRACE("nn->data is not NULL but check return 0\n");
                    if (0 == ret) {
                        continue;
                    }
                    else if (ret > 0) {
                        len = pcb(nn->data, ret, cache, sizeof(cache));
                        if (ret < nn->len) { // next msg is here~ 
                            int left = nn->len - ret;
                            memmove(nn->data, nn->data + ret, left);
                            nn->len = left;
                        } 
                        else {
                            free(nn->data);
                            nn->data = NULL;
                            nn->len  = 0;
                        }
                    }
                }

                // handle respone
                if (0 == len) { // no respone, continue recv
                    continue;
                }
                else if (len > 0) {
                    if (nn->sdata) free(nn->sdata);

                    nn->sdata = (char*)malloc(len);
                    nn->start = 0;
                    nn->slen  = len;
                    memcpy(nn->sdata, cache, len);
                    hpev[i].events = EPOLLOUT | EPOLLET;
                    epoll_ctl(epfd, EPOLL_CTL_MOD, hpev[i].data.fd, &hpev[i]);
                }
            }
            else if (hpev[i].events & EPOLLOUT) {   // write
                HashNode* nn = HT_getx(&ht, hpev[i].data.fd);
                if (!nn->sdata) {
                    ev.data.fd = hpev[i].data.fd;
                    ev.events  = EPOLLIN|EPOLLET;
                    epoll_ctl(epfd, EPOLL_CTL_MOD, ev.data.fd, &ev);
                }
                // assert nn->key == hpev[i].data.fd
                int n = write(nn->key, nn->sdata+nn->start, nn->slen);
                // write all data 
                if (n == nn->slen) {
                    free(nn->sdata);
                    nn->sdata = NULL;
                    nn->start = 0;
                    nn->slen  = 0;
                    // change to EPOLLOUT
                    ev.data.fd = hpev[i].data.fd;
                    ev.events  = EPOLLIN|EPOLLET;
                    epoll_ctl(epfd, EPOLL_CTL_MOD, ev.data.fd, &ev);
                }
                else if (n < nn->slen) {
                    nn->start += n;
                    nn->slen  -= n;
                }
            }
        }
    }

    return NULL;
}

