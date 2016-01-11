/* **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : 1.c
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-05 15:40:53
 *  Version : 0.01
 * **************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../hashtable.h"
#include <vector>
using namespace std;

#define MAX 100

int main(int argc, char const* argv[])
{
    srand(time(0)^getpid());
    vector<int> vi;
    
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    HT_create(ht);

    char buff[2048];
    int i = 0;
    for (; i < MAX; ++i) {
        char hh[32];
        int  rr = rand();
        vi.push_back(rr);
        sprintf(hh, "%d", rr);

        HT_set(ht, rr, hh, strlen(hh));
        // printf("i is %d\n", i);
    }

    HT_show(ht);

    i = 0;
    for (auto x:vi) {
        ++i;
        HashNode* hh = HT_get(ht, x);
        if (hh) {
            printf("[%s]\n", hh->data);
        }

        HT_del(ht, x);
        // usleep(100000);
    }

    HT_info(ht);
    HT_destory(ht);
    free(ht);

    return 0;
}

