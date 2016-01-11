/* **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : hashtable.c
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-05 11:36:17
 *  Version : 0.01
 * **************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <jemalloc/jemalloc.h>
#include "internal.h"
#include "hashtable.h"

static unsigned int prime_array[] = {                    
    17,
    37,
    79,
    163,
    331,
    673,
    1361,
    2729,
    5471,
    10949,
    21911,
    43853,
    87719,
    175447,
    350899,
    701819,
    1403641,
    2807303,
    5614657,
    11229331,
    22458671,
    44917381,
    89834777,
    179669557,
    359339171,
    718678369,
    1437356741,
    2147483647
}; 

unsigned int BKDRHash(const char* key, int key_len) {
    unsigned char *pKey;
    unsigned char *pEnd;

    unsigned int seed = 131;
    unsigned int hash = 0;

    pEnd = (unsigned char *)key + key_len;
    for (pKey = (unsigned char *)key; pKey != pEnd; pKey++) {
        hash = hash * seed + (*pKey);
    }

    return hash;
}

#define INC_FACTOR 0.6
#define DEC_FACTOR 0.2

#define HASHFUNC(x, y) BKDRHash(x, y) 

void HT_create(HashTable* ht) {
    TRACE("bucket size %d\n", prime_array[ht->index]);
    ht->index   = 0;
    ht->buckets = prime_array[ht->index];
    ht->size    = 0;
    ht->base = (HashNode**)calloc(ht->buckets, sizeof(HashNode*));
    TRACE("create over!\n");
}

// not exist, intsert
// existed,   append data to existed node
void HT_append(HashTable* ht, int key, const char* data, int data_len) {
    ++ht->size;

    if (ht->size > INC_FACTOR*prime_array[ht->index]) {
        HT_rehash(ht, 0);
    }

    char strkey[16];
    sprintf(strkey, "%d", key);
    unsigned int tag = HASHFUNC(strkey, strlen(strkey));
    unsigned int index = tag%(ht->buckets);

    TRACE("Handle f*ck list, tag [%u], index [%u], bucket [%u], size [%d]\n", tag, index, ht->buckets, ht->size);

    // bucket[index] is empty
    if (!(ht->base[index])) {
        ht->base[index] = (HashNode*)malloc(sizeof(HashNode));
        HashNode* curr = ht->base[index];
        curr->key  = key;
        curr->tag  = tag;
        curr->len  = data_len;
        curr->data = (char*)malloc(data_len);
        memcpy(curr->data, data, data_len);
        curr->next = NULL;
        return;
    }

    // bucket[index] is not empty
    HashNode* ps = ht->base[index];
    for (;;) {
        if (ps->key == key) {
            if (ps->data) {
                char* newdata = (char*)realloc(ps->data, ps->len + data_len);
                memcpy(newdata+ps->len, data, data_len);
                ps->len  = ps->len + data_len;
                ps->data = newdata;
            }
            else {
                ps->data = (char*)malloc(data_len);
                ps->len  = data_len;
                memcpy(ps->data, data, data_len);
            }
            break;
        }
        else if (ps->key != key && !ps->next) {     // not existed
            HashNode* newnode = (HashNode*)calloc(1, sizeof(HashNode));
            newnode->key  = key;
            newnode->tag  = tag;
            newnode->data = (char*)malloc(data_len);
            newnode->len  = data_len;
            memcpy(newnode->data, data, data_len);
            ps->next = newnode;
            break;
        }
        else {
            ps = ps->next;
        }
    }
}

// not exist, intsert
// existed,   append data to existed node
void HT_set(HashTable* ht, int key, const char* data, int data_len) {
    if (ht->size > INC_FACTOR*prime_array[ht->index]) {
        HT_rehash(ht, 0);
    }

    char strkey[16];
    sprintf(strkey, "%d", key);
    unsigned int tag = HASHFUNC(strkey, strlen(strkey));
    unsigned int index = tag%(ht->buckets);

    TRACE("Handle f*ck list, tag [%u], index [%u], bucket [%u], size [%d]\n", tag, index, ht->buckets, ht->size);

    // bucket[index] is empty
    if (!(ht->base[index])) {
        ht->base[index] = (HashNode*)malloc(sizeof(HashNode));
        HashNode* curr = ht->base[index];
        curr->key  = key;
        curr->tag  = tag;
        curr->len  = data_len;
        curr->data = (char*)malloc(data_len);
        memcpy(curr->data, data, data_len);
        curr->next = NULL;
        ++ht->size;
        return;
    }

    // bucket[index] is not empty
    HashNode* ps = ht->base[index];
    for (;;) {
        if (ps->key == key) {
            if (ps->data) {
                if (ps->len < data_len) {
                    char* newdata = (char*)realloc(ps->data, ps->len + data_len);
                    ps->data = newdata;
                }
            }
            else {
                ps->data = (char*)malloc(data_len);
            }
            ps->len  = data_len;
            memcpy(ps->data, data, data_len);
            break;
        }
        else if (ps->key != key && !ps->next) {     // not existed
            HashNode* newnode = (HashNode*)calloc(1, sizeof(HashNode));
            newnode->key  = key;
            newnode->tag  = tag;
            newnode->data = (char*)malloc(data_len);
            newnode->len  = data_len;
            newnode->next = NULL;
            memcpy(newnode->data, data, data_len);
            ps->next = newnode;
            break;
        }
        else {
            ps = ps->next;
        }
    }

    ++ht->size;
}

// just get data
HashNode* HT_get(const HashTable* ht, int key) {
    char strkey[16];
    sprintf(strkey, "%d", key);
    unsigned int tag = HASHFUNC(strkey, strlen(strkey));
    unsigned int index = tag%(ht->buckets);

    HashNode* ps = ht->base[index];

    if (!ps) return NULL;

    for (; ps; ps = ps->next) {
        if (ps->key == key) {
            return ps;
        }
    }

    return NULL;
}

// return cannot be null
HashNode* HT_getx(const HashTable* ht, int key) {
    char strkey[16];
    sprintf(strkey, "%d", key);
    unsigned int tag = HASHFUNC(strkey, strlen(strkey));
    unsigned int index = tag%(ht->buckets);

    HashNode* ps = ht->base[index];
    
    for (; ps; ps = ps->next) {
        if (ps->key == key) {
            return ps;
        }
    }
    
    ps = (HashNode*)malloc(sizeof(HashNode));
    ps->key   = key;
    ps->tag   = tag;
    ps->data  = NULL;
    ps->len   = 0;
    ps->start = 0;
    ps->slen  = 0;
    ps->sdata = NULL;
    ps->next  = ht->base[index];
    ht->base[index] = ps;
    return ps;
}

void HT_del(HashTable* ht, int key) {
    char strkey[16];
    sprintf(strkey, "%d", key);
    unsigned int tag = HASHFUNC(strkey, strlen(strkey));
    unsigned int index = tag%(ht->buckets);

    HashNode* ps  = ht->base[index];

    if (!ps) return;

    if (ps->key == key) {
        free(ps->data);
        ps = ps->next;
        free(ht->base[index]);
        ht->base[index] = ps;
        --ht->size;
        if (ht->index < 3) 
            return;

        if (ht->size < DEC_FACTOR*prime_array[ht->index])
            HT_rehash(ht, 1);

        return;
    }

    for (;ps; ps = ps->next) {
        if (ps->next && ps->next->key == key) {
            free(ps->next->data);
            HashNode* tp = ps->next;
            ps->next = ps->next->next;
            free(tp);
            --ht->size;
            if (ht->index < 3) 
                return;

            if (ht->size < DEC_FACTOR*prime_array[ht->index])
                HT_rehash(ht, 1);

            return;
        }
    }
}

void HT_rehash(HashTable* ht, int how) {

    int oldbuckets = prime_array[ht->index];
    if (0 == how) {
        TRACE("reahsh inc");
        ++ht->index;
    }
    else if (1 == how) {
        TRACE("rehash dec");
        if (0 == ht->index) return;

        --ht->index;
    }

    HashNode** newbase = (HashNode**)calloc(prime_array[ht->index], sizeof(HashNode*));

    int i = 0;
    HashNode* ps1 = NULL;
    HashNode* ps2 = NULL;
    for (; i < oldbuckets; ++i) {
        if (!ht->base[i]) continue;

        for (ps1 = ht->base[i]; ps1;) {
            int newindex = ps1->tag%(prime_array[ht->index]);
            ps2 = newbase[newindex];
            newbase[newindex] = ps1;
            ps1 = ps1->next;
            newbase[newindex]->next = ps2;
        }
        ht->base[i] = NULL;
    }

    free (ht->base);
    ht->base = newbase;
    ht->buckets = prime_array[ht->index];

    HT_info(ht);
}

void HT_destory(HashTable* ht) {
    int buckets = prime_array[ht->index];

    int i = 0;
    for (; i < buckets; ++i) {
        if (!ht->base[i]) continue;

        HashNode* ps1 = NULL;
        HashNode* ps2 = ps1;
        for (ps1 = ht->base[i]; ps1;) {
            ps2 = ps1;
            ps1 = ps1->next;
            free(ps2->data);
            free(ps2);
        }
    }

    free(ht->base);
    ht->base  = NULL;
    ht->size  = 0;
    ht->index = 0;
}

int   HT_size(const HashTable* ht) {
    return ht->size;
}

void HT_show(const HashTable* ht) {
    printf("-------------------------------------------------------------\n");
    int cnt = ht->buckets;
    int i = 0;
    for (; i < cnt; ++i) {
        HashNode* ps = ht->base[i];
        if (!ps) continue;
        printf("B[%03d]:", i);
        while (ps) {
            printf("10%d,", ps->key);
            ps = ps->next;
        }

        printf("\n");
    }
}

void HT_info(const HashTable* ht) {
    printf("index[%d], buckets[%03d], size[%03d]\n", ht->index, ht->buckets, ht->size);
}
