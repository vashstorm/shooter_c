/* **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : queue.c
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-06 14:11:20
 *  Version : 0.01
 * **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <jemalloc/jemalloc.h>
#include "queue.h"

void shq_init(SH_QUEUE* sq) {
    sq->size = 0;
    sq->head = NULL;
    sq->tail = NULL;
}

void shq_push(SH_QUEUE* sq, int fd) {
    if (sq->tail) {
        SH_QNODE* newnode = malloc(sizeof(SH_QUEUE));
        newnode->fd   = fd;
        newnode->next = NULL;
        sq->tail->next = newnode;
        sq->tail       = newnode;
    }
    else {
        SH_QNODE* newnode = malloc(sizeof(SH_QUEUE));
        newnode->fd    = fd;
        newnode->next  = NULL;
        sq->head       = newnode;
        sq->tail       = newnode;
    }
    
    ++sq->size;
}

int  shq_pop(SH_QUEUE* sq, int* res) {
   if (sq->size <= 0) {
       return -1;
   }
   else if (1 == sq->size) {
       *res = sq->head->fd;
        free(sq->head);
        sq->head = NULL;
        sq->tail = NULL;
   }
   else { // > 1
        SH_QNODE* ps = sq->head->next;
        *res = sq->head->fd;
        free(sq->head);
        sq->head = ps;
   }

   return --sq->size;
}

int  shq_size(SH_QUEUE* sq) {
    return sq->size;
}

void shq_destory(SH_QUEUE* sq) {
    SH_QNODE* ps1 = sq->head;
    SH_QNODE* ps2 = sq->head;
    while (ps1) {
        ps2 = ps1->next;
        free(ps1);
        ps1 = ps2;
    }
    sq->size = 0;
    sq->head = NULL;
    sq->tail = NULL;
}

