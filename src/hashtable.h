/* **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : hashtable.h
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-05 11:36:17
 *  Version : 0.01
 * **************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

// just a list
typedef struct hash_node {
    int         key;    // file descriptor
    int         tag;    // val after hash
    int         len;    // length of data
    int         start;  // position to write
    int         slen;   // length   to write
    char*       data;   // data of receive
    char*       sdata;  // data to send
    struct hash_node* next;
} HashNode;

typedef struct hash_table {
    int         index;      // for prime_array 
    int         buckets;    // size of buckets
    int         size;       // size of elements
    HashNode**  base;       
} HashTable;

void  HT_create(HashTable* ht);

// not exist, intsert
// existed,   append data to existed node
void  HT_append(HashTable* ht, int key, const char* data, int data_len);

void  HT_set(HashTable* ht, int key, const char* data, int data_len);

// just get data
HashNode* HT_get(const HashTable* ht, int key);

HashNode* HT_getx(const HashTable* ht, int key);

void  HT_del(HashTable* ht, int key);

void  HT_rehash(HashTable* ht, int how);

void  HT_destory(HashTable* ht);

void  HT_show(const HashTable* ht);

void  HT_info(const HashTable* ht);

int   HT_size(const HashTable* ht);

#ifdef __cplusplus
}
#endif


