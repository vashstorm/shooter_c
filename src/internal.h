/* **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : internal.h
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-05 10:32:09
 *  Version : 0.01
 * **************************************************************/

#ifndef __SHOOTER_INTERNAL_H__
#define __SHOOTER_INTERNAL_H__

#ifdef DEBUG
#define TRACE(...) \
    { \
        printf("[%-16s %3d] ", __FILE__, __LINE__); \
        printf(__VA_ARGS__); \
    }
#else
#define TRACE(...)
#endif

// return: 0, donot has respone msg
//         1, respone msg is in odata, length is olen
typedef int (*shooter_callback)(const char* indata, int ilen, char* odata, int olen);

typedef struct ev_and_cb {
    int               ev;
    shooter_callback  cb;  
} EVCB;

#endif /*  end of include guard */
