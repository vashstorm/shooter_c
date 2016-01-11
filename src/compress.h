/* **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : compress.h
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-04 16:04:30
 *  Version : 0.01
 * **************************************************************/

#ifndef __SHOOTER_COMPRESS_H__
#define __SHOOTER_COMPRESS_H__

#ifdef __cplusplus
extern "C" {      
#endif            

int shooter_compress(const char* src, int srcLen, char* dst, int dstLen);

int shooter_decompress(const char* src, int srcLen, char* dst, int dstLen);

#ifdef __cplusplus
}
#endif            

#endif

