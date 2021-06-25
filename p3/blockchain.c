/*********************************************************************
Homework 5
CS 110: Computer Architecture, Spring 2021
ShanghaiTech University

* Last Modified: 03/28/2021
*********************************************************************/

#include "blockchain.h"
#include <string.h>
#include <stdint.h>
#include <omp.h>

typedef unsigned long long ll;

void blockchain_node_init(blk_t *node, uint32_t index, uint32_t timestamp,
                          unsigned char prev_hash[32], unsigned char *data,
                          size_t data_size) {
    if (!node || !data || !prev_hash)
        return;

    node->header.index = index;
    node->header.timestamp = timestamp;
    node->header.nonce = -1;

    memset(node->header.data, 0, sizeof(unsigned char) * 256);
    memcpy(node->header.prev_hash, prev_hash, HASH_BLOCK_SIZE);
    memcpy(node->header.data, data,
           sizeof(unsigned char) * ((data_size < 256) ? data_size : 256));
}

struct myStruct {
    blk_t *node;
    unsigned char *hash_buf;
    size_t diff_q;
    hash_func *func;
    unsigned char *one_diff;
};

struct MySwitch {
    BOOL finish;
    // uint64_t thread_nonce;
} myswithch;

ll global_nouce = (-1);


void blockchain_node_hash(blk_t *node, unsigned char hash_buf[HASH_BLOCK_SIZE],
                          hash_func func) {
    if (node)
        func((unsigned char *) node, sizeof(blkh_t), (unsigned char *) hash_buf);
}

BOOL blockchain_node_verify(blk_t *node, blk_t *prev_node, hash_func func) {
    unsigned char hash_buf[HASH_BLOCK_SIZE];

    if (!node || !prev_node)
        return False;

    blockchain_node_hash(node, hash_buf, func);
    if (memcmp(node->hash, hash_buf, sizeof(unsigned char) * HASH_BLOCK_SIZE))
        return False;

    blockchain_node_hash(prev_node, hash_buf, func);
    if (memcmp(node->header.prev_hash, hash_buf,
               sizeof(unsigned char) * HASH_BLOCK_SIZE))
        return False;

    return True;
}

void myStructInit(struct myStruct *my_struct, blk_t *node, unsigned char *hash_buf,
                  size_t diff_q, hash_func func, unsigned char *one_diff) {
    my_struct->node = node;
    my_struct->hash_buf = hash_buf;
    my_struct->diff_q = diff_q;
    my_struct->func = func;
    my_struct->one_diff = one_diff;
}

/* The sequiental implementation of mining implemented for you. */
void blockchain_node_mine(blk_t *node, unsigned char hash_buf[HASH_BLOCK_SIZE],
                          size_t diff, hash_func func) {
    unsigned char one_diff[HASH_BLOCK_SIZE];
    size_t diff_q, diff_m;
    /* struct myStruct my_struct={node,hash_buf,diff_q,func,one_diff}; */
    struct myStruct my_struct;

    diff_q = diff / 8;
    diff_m = diff % 8;
    unsigned long long int temp = -1;
    unsigned long long int nonce_trick = temp / 20;
    memset(one_diff, 0xFF, sizeof(unsigned char) * HASH_BLOCK_SIZE);
    memset(one_diff, 0, sizeof(unsigned char) * diff_q);
    one_diff[diff_q] = ((uint8_t) 0xFF) >> diff_m;
    myswithch.finish = False;
    myStructInit(&my_struct, node, hash_buf, diff_q, func, one_diff);

#pragma omp parallel num_threads(4)
    {
        unsigned char t_hash_buf[HASH_BLOCK_SIZE];
        blk_t thread_node;
        blockchain_node_init(&thread_node, node->header.index, node->header.timestamp,
                             node->header.prev_hash, node->header.data,
                             sizeof(node->header.data));
#pragma omp critical
        {
            thread_node.header.nonce = global_nouce;
            global_nouce += nonce_trick;
        }

        blk_t *midvar1 = &thread_node;
        hash_func *midvar2 = func;
        unsigned char *midvar3 = one_diff;
        unsigned long midvar4 = sizeof(unsigned char) * diff_q;
        unsigned char *midvar5 = &t_hash_buf[diff_q];
        unsigned char *midvar6 = &one_diff[diff_q];
        unsigned long midvar7 = sizeof(unsigned char) * (HASH_BLOCK_SIZE - diff_q);

        while (myswithch.finish == False) {
            blockchain_node_hash(midvar1, t_hash_buf, midvar2);
            if ((!memcmp(t_hash_buf, midvar3, midvar4)) &&
                memcmp(midvar5, midvar6, midvar7) <= 0) {
                if (myswithch.finish == False) {
                    myswithch.finish = True;
                    node->header.nonce = thread_node.header.nonce;
                    memcpy(node->hash, t_hash_buf, sizeof(unsigned char) * HASH_BLOCK_SIZE);
                    memcpy(hash_buf, t_hash_buf, sizeof(unsigned char) * HASH_BLOCK_SIZE);
                }
                break;
            }
            thread_node.header.nonce += 1;
        }
    }
}



