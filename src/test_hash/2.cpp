/* **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : 2.cpp
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

        HT_append(ht, rr, hh, strlen(hh));
        // printf("i is %d\n", i);
        usleep(100000);

        if ((i+1)%10 == 0) 
            HT_show(ht);
    }

    printf("\n\n\n################################################################\n\n\n");

    i = 0;
    for (auto x:vi) {
        ++i;
        HT_del(ht, x);
        // printf("i is %d\n", i);
        usleep(100000);

        if ((i+1)%10 == 0) 
            HT_show(ht);
    }

    HT_info(ht);
    HT_destory(ht);
    free(ht);

    return 0;
}

