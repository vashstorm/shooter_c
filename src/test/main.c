/* **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : main.c
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-06 21:55:14
 *  Version : 0.01
 * **************************************************************/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "../server.h"

int hhh(const char* indata, int ilen, char* odata, int olen) {
    time_t tt = time(0);
    struct tm ttm;
    gmtime_r(&tt, &ttm);
    snprintf(odata, olen, "%d-%02d-%02d %02d:%02d:%02d\0", ttm.tm_year+1900, \
            ttm.tm_mon+1, ttm.tm_mday, ttm.tm_hour, ttm.tm_min, ttm.tm_sec);

    return strlen(odata);
}

int main(int argc, char const* argv[])
{
    int i = shooter_init("127.0.0.1", 9865, hhh);
    printf("bind port [%d]\n", i);
    if (!i) {
        return -1;
    }

    shooter_set("thread_num", 1);
    shooter_run();

    puts("hello, world!");
    return 0;
}
