/* **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : package.c
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-04 21:26:37
 *  Version : 0.01
 * **************************************************************/

#include <stdint.h>
#include <string.h>
#include "package.h"

// char     version
// char     compress 
// int32_t  len
int shooter_check_complete(const char* data, int len) {
    int  headlen = 2*sizeof(char) + sizeof(int32_t);
    if (len < headlen) return 0;

    int  bodylen;
    memcpy(&bodylen, data + 2, sizeof(bodylen));
    if (len >= (bodylen + headlen)) {
        return bodylen + headlen;
    }
    else {
        return 0;
    }
}

int shooter_check_complete_line(const char* data, int len) {
    int i = 0 ;
    for (; i < len; ++i) {
        if (data[i] == '\n') {
            return ++i;
        }
    }

    if (i == len) return 0;
}

int shooter_pack(const char* in, int ilen, char* out, int olen) {
    int  headlen = 2*sizeof(char) + sizeof(int32_t);
    if ((ilen + headlen) < olen)  return -1;
    *out = 1;
    *(out+1) = 0;
    memcpy(out+2, &ilen, sizeof(ilen));
    memcpy(out+headlen, in, ilen);  

    return ilen + headlen;
}

int shooter_unpack(const char* in, int ilen, char* out, int olen) {
    int  headlen = 2*sizeof(char) + sizeof(int32_t);
    if (olen < (ilen - headlen)) return -1;
    int len;
    memcpy(&len, in+2, sizeof(ilen));
    memcpy(out, in + headlen, len);   
    
    return len;
}


