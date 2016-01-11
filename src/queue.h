/* **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : queue.h
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-06 14:29:26
 *  Version : 0.01
 * **************************************************************/

#ifndef __SHOOTER_QUEUE_H__
#define __SHOOTER_QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sh_queue_node {
    int    fd;
    struct sh_queue_node* next;
} SH_QNODE;

typedef struct sh_queue {
    int         size;
    SH_QNODE*   head;
    SH_QNODE*   tail;
} SH_QUEUE;

void shq_init(SH_QUEUE *sq);

void shq_push(SH_QUEUE *sq, int fd);

int  shq_pop(SH_QUEUE *sq, int *res);

int  shq_size(SH_QUEUE *sq);

void shq_destory(SH_QUEUE *sq);

#ifdef __cplusplus
}
#endif

#endif /*  end of include guard */

