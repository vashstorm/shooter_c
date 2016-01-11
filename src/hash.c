#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

// RS Hash Function
int RSHash(const void *key, const int key_len)
{
    unsigned char *pKey;
    unsigned char *pEnd;
    int a = 63689;
    int hash = 0;

    pEnd = (unsigned char *)key + key_len;
    for (pKey = (unsigned char *)key; pKey != pEnd; pKey++)
    {
        hash = hash * a + (*pKey);
        a *= 378551;
    }

    return hash;
} 
 
#define JS_HASH_FUNC(init_value) \
    unsigned char *pKey; \
    unsigned char *pEnd; \
    int hash; \
 \
    hash = init_value; \
    pEnd = (unsigned char *)key + key_len; \
    for (pKey = (unsigned char *)key; pKey != pEnd; pKey++) \
    { \
        hash ^= ((hash << 5) + (*pKey) + (hash >> 2)); \
    } \
 \
    return hash; \


// JS Hash Function
int JSHash(const void *key, const int key_len)
{
	JS_HASH_FUNC(1315423911)
}
 
int JSHash_ex(const void *key, const int key_len, \
	const int init_value)
{
	JS_HASH_FUNC(init_value)
}
 
#define BITS_IN_UNIGNED_INT (int)(sizeof(int) * 8)
#define THREE_QUARTERS      (int)((BITS_IN_UNIGNED_INT * 3) / 4)
#define HASH_ONE_EIGHTH     (int)(BITS_IN_UNIGNED_INT / 8)
#define HASH_HIGH_BITS      (int)((unsigned int)(0xFFFFFFFF) << \
				(BITS_IN_UNIGNED_INT - HASH_ONE_EIGHTH))

#define PJW_HASH_FUNC(init_value) \
    unsigned char *pKey; \
    unsigned char *pEnd; \
    int hash; \
    int test; \
 \
    hash = init_value; \
    pEnd = (unsigned char *)key + key_len; \
    for (pKey = (unsigned char *)key; pKey != pEnd; pKey++) \
    { \
        hash = (hash << HASH_ONE_EIGHTH) + (*(pKey)); \
        if ((test = hash & HASH_HIGH_BITS) != 0) \
        { \
            hash = ((hash ^ (test >> THREE_QUARTERS)) & (~HASH_HIGH_BITS)); \
        } \
    } \
 \
    return hash; \


// P.J.Weinberger Hash Function, same as ELF Hash
int PJWHash(const void *key, const int key_len)
{
	PJW_HASH_FUNC(0)
}
 
int PJWHash_ex(const void *key, const int key_len, \
	const int init_value)
{
	PJW_HASH_FUNC(init_value)
}
 
#define ELF_HASH_FUNC(init_value) \
    unsigned char *pKey; \
    unsigned char *pEnd; \
    int hash; \
    int x; \
 \
    hash = init_value; \
    pEnd = (unsigned char *)key + key_len; \
    for (pKey = (unsigned char *)key; pKey != pEnd; pKey++) \
    { \
        hash = (hash << 4) + (*pKey); \
        if ((x = hash & 0xF0000000) != 0) \
        { \
            hash ^= (x >> 24); \
            hash &= ~x; \
        } \
    } \
 \
    return hash; \


// ELF Hash Function, same as PJW Hash
int ELFHash(const void *key, const int key_len)
{
	ELF_HASH_FUNC(0)
}

int ELFHash_ex(const void *key, const int key_len, \
	const int init_value)
{
	ELF_HASH_FUNC(init_value)
}

#define BKDR_HASH_FUNC(init_value) \
    unsigned char *pKey; \
    unsigned char *pEnd; \
    int seed = 131;  /* 31 131 1313 13131 131313 etc..*/ \
    int hash; \
 \
    hash = init_value; \
    pEnd = (unsigned char *)key + key_len; \
    for (pKey = (unsigned char *)key; pKey != pEnd; pKey++) \
    { \
        hash = hash * seed + (*pKey); \
    } \
 \
    return hash; \


// BKDR Hash Function
int BKDRHash(const void *key, const int key_len)
{
	BKDR_HASH_FUNC(0)
}

int BKDRHash_ex(const void *key, const int key_len, \
	const int init_value)
{
	BKDR_HASH_FUNC(init_value)
}

#define SDBM_HASH_FUNC(init_value) \
    unsigned char *pKey; \
    unsigned char *pEnd; \
    int hash; \
 \
    hash = init_value; \
    pEnd = (unsigned char *)key + key_len; \
    for (pKey = (unsigned char *)key; pKey != pEnd; pKey++) \
    { \
        hash = (*pKey) + (hash << 6) + (hash << 16) - hash; \
    } \
 \
    return hash; \


// SDBM Hash Function
int SDBMHash(const void *key, const int key_len)
{
	SDBM_HASH_FUNC(0)
}

int SDBMHash_ex(const void *key, const int key_len, \
	const int init_value)
{
	SDBM_HASH_FUNC(init_value)
}

#define TIME33_HASH_FUNC(init_value) \
	int nHash; \
	unsigned char *pKey; \
	unsigned char *pEnd; \
 \
	nHash = init_value; \
	pEnd = (unsigned char *)key + key_len; \
	for (pKey = (unsigned char *)key; pKey != pEnd; pKey++) \
	{ \
		nHash += (nHash << 5) + (*pKey); \
	} \
 \
	return nHash; \


int Time33Hash(const void *key, const int key_len)
{
	TIME33_HASH_FUNC(0)
}

int Time33Hash_ex(const void *key, const int key_len, \
	const int init_value)
{
	TIME33_HASH_FUNC(init_value)
}

#define DJB_HASH_FUNC(init_value) \
    unsigned char *pKey; \
    unsigned char *pEnd; \
    int hash; \
 \
    hash = init_value; \
    pEnd = (unsigned char *)key + key_len; \
    for (pKey = (unsigned char *)key; pKey != pEnd; pKey++) \
    { \
        hash += (hash << 5) + (*pKey); \
    } \
 \
    return hash; \


// DJB Hash Function
int DJBHash(const void *key, const int key_len)
{
	DJB_HASH_FUNC(5381)
}

int DJBHash_ex(const void *key, const int key_len, \
	const int init_value)
{
	DJB_HASH_FUNC(init_value)
}

#define AP_HASH_FUNC(init_value) \
    unsigned char *pKey; \
    unsigned char *pEnd; \
    int i; \
    int hash; \
 \
    hash = init_value; \
 \
    pEnd = (unsigned char *)key + key_len; \
    for (pKey = (unsigned char *)key, i=0; pKey != pEnd; pKey++, i++) \
    { \
        if ((i & 1) == 0) \
        { \
            hash ^= ((hash << 7) ^ (*pKey) ^ (hash >> 3)); \
        } \
        else \
        { \
            hash ^= (~((hash << 11) ^ (*pKey) ^ (hash >> 5))); \
        } \
    } \
 \
    return hash; \


// AP Hash Function
int APHash(const void *key, const int key_len)
{
	AP_HASH_FUNC(0)
}

int APHash_ex(const void *key, const int key_len, \
	const int init_value)
{
	AP_HASH_FUNC(init_value)
}

