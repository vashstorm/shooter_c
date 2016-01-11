/*  **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : server.h
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-04 14:21:48
 *  Version : 0.01
 ************************************************************** */

#ifndef __SHOOTER_SERVER_H__
#define __SHOOTER_SERVER_H__

// return: 0, donot has respone msg
//         1, respone msg is in odata, length is olen
typedef int (*shooter_callback)(const char* indata, int ilen, char* odata, int olen);

// server_info > 0, host is ip
// server_info = 0, host is unix socket
int  shooter_init(const char *host, int server_info, shooter_callback cb);
void shooter_run(void);
void shooter_set(const char *name, int val);

#endif /*  end of include guard */
