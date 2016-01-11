/*  **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : compress.h
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-04 16:04:30
 *  Version : 0.01
 ************************************************************** */

#ifndef __SHOOTER_COMPRESS_H__
#define __SHOOTER_COMPRESS_H__

#include <zlib.h>

int shooter_compress(const char* src, int srcLen, char* dst, int dstLen)
{
    z_stream strm;
    // zalloc, zfree, opaque, use default
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    strm.avail_in = srcLen;
    strm.avail_out = dstLen;
    strm.next_in = (Bytef*)src;
    strm.next_out = (Bytef*)dst;

    int err = -1;
    // init z_stream
    // deflateInit:zlib, deflateInit2:gzip
    // err = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    err = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY);

    if (err == Z_OK) {
        err = deflate(&strm, Z_FINISH);

        if (err == Z_STREAM_END) {
            deflateEnd(&strm);
            return dstLen - strm.avail_out;
        }
        else {
            deflateEnd(&strm);
            m_ErrMsg = "compression failed, deflate return: ";
            return -1;
        }
    }
    else {
        (void)deflateEnd(&strm);
        m_ErrMsg = "compression initialization failed, quit!";
        return 0;
    }
}

int shooter_decompress(const char* src, int srcLen, char* dst, int dstLen)
{
    z_stream strm;
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;

    strm.avail_in = srcLen;
    strm.avail_out = dstLen;
    strm.next_in = (Bytef*)src;
    strm.next_out = (Bytef*)dst;

    int err = -1;
    err = inflateInit2(&strm, MAX_WBITS + 16);

    // err = inflateInit(&strm);
    if (err == Z_OK) {
        err = inflate(&strm, Z_FINISH);

        if (err == Z_STREAM_END) {
            (void)inflateEnd(&strm);
            return strm.total_out;
        }
        else {
            (void)inflateEnd(&strm);
            return -1;
        }
    }
    else {
        inflateEnd(&strm);
        m_ErrMsg = "decompression initialization failed, quit!";
        return 0;
    }
}

#endif

