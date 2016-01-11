/* **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : package.h
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-04 21:26:37
 *  Version : 0.01
 * **************************************************************/

#ifndef __SHOOTER_PACKAGE_H__
#define __SHOOTER_PACKAGE_H__

int shooter_check_complete(const char* d, int len);

int shooter_check_complete_line(const char* data, int len);

int shooter_pack(const char *in, int ilen, char *out, int olen);

int shooter_unpack(const char *in, int ilen, char *out, int olen);

#endif /*  end of include guard */
