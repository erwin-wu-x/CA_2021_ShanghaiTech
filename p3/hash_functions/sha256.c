/*********************************************************************
Homework 5
CS 110: Computer Architecture, Spring 2021
ShanghaiTech University

* Last Modified: 03/28/2021
* Copyright Notice: This file is dervived from work of Brad Conte at
  https://github.com/B-Con/crypto-algorithms/
*********************************************************************/

/*********************************************************************
* Filename:   sha256.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Implementation of the SHA-256 hashing algorithm.
              SHA-256 is one of the three algorithms in the SHA2
              specification. The others, SHA-384 and SHA-512, are not
              offered in this implementation.
              Algorithm specification can be found here:
               * http://csrc.nist.gov/publications/fips/fips180-2/fips180-2withchangenotice.pdf
              This implementation uses little endian byte order.
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include "sha256.h"
#include <memory.h>
#include <stdlib.h>
#include <omp.h>
#include <emmintrin.h> /* header file for the SSE intrinsics */
#include <smmintrin.h>
#include <immintrin.h>
// #define DEBUGMODE
#define MYSSE
#ifdef DEBUGMODE
#include <stdio.h>
#include <stdint.h>
void print128_num(__m128i var) 
{ 
    int32_t *v32val = (int32_t*) &var; 
    printf("%.16x %.16x %.16x %.16x\n", v32val[0], v32val[1], v32val[2], v32val[3]); 
} 
void decToBin(WORD num)
{
	if(num>0)
		{
			decToBin(num/2);
			printf("%u",num%2);
		}

}

#endif

/****************************** MACROS ******************************/
#define ROTLEFT(a, b) (((a) << (b)) | ((a) >> (32 - (b))))
#define ROTRIGHT(a, b) (((a) >> (b)) | ((a) << (32 - (b))))

#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
#define EP1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))
#define SIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))

/**************************** VARIABLES *****************************/
static const WORD k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
        0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
        0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
        0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
        0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
        0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

/*********************** FUNCTION DEFINITIONS ***********************/
void sha256_transform(SHA256_CTX *ctx, const BYTE data[]) {
    WORD  i, j, t1, t2, m[64];

    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) |
               (data[j + 3]);
    for (; i < 64; ++i)
        m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];
    #ifndef MYSSE
    WORD a, b, c, d, e, f, g, h;
    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    f = ctx->state[5];
    g = ctx->state[6];
    h = ctx->state[7];

    for (i = 0; i < 64; ++i) {
        t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
        t2 = EP0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
    #endif
    #ifdef MYSSE
    //my_ptr[0]: my_ptr[0]
    //my_ptr[1]: my_ptr[1]
    //my_ptr[2]: my_ptr[2]
    //my_ptr[3]: my_ptr[3]
    //my_ptr[4]: my_ptr[4]
    //my_ptr[5]: my_ptr[5]
    //my_ptr[6]: my_ptr[6]
    //my_ptr[7]: my_ptr[7]
    __m256i myState = _mm256_loadu_si256((__m256i*) ctx->state);
    WORD *my_ptr = (WORD*)&myState;
    for (i = 0; i < 64; ++i) {
        t1 = my_ptr[7] + EP1(my_ptr[4]) + CH(my_ptr[4], my_ptr[5], my_ptr[6]) + k[i] + m[i];
        t2 = EP0(my_ptr[0]) + MAJ(my_ptr[0], my_ptr[1], my_ptr[2]);
        my_ptr[7] = my_ptr[6];
        my_ptr[6] = my_ptr[5];
        my_ptr[5] = my_ptr[4];
        my_ptr[4] = my_ptr[3] + t1;
        my_ptr[3] = my_ptr[2];
        my_ptr[2] = my_ptr[1];
        my_ptr[1] = my_ptr[0];
        my_ptr[0] = t1 + t2;
    }
    // _mm256_storeu_si256((__m256i*) ctx->state,myState);
    _mm256_storeu_si256((__m256i*) ctx->state,_mm256_add_epi32(_mm256_loadu_si256((__m256i*) ctx->state),myState));


    #endif
}


void sha256_init(SHA256_CTX *ctx) {
    ctx->datalen = 0;
    ctx->bitlen = 0;
    
    // _mm256_storeu_si256((__m256i *)ctx->state,_mm256_set_epi32(0x5be0cd19,0x1f83d9ab,0x9b05688c,0x510e527f,0xa54ff53a,0x3c6ef372,0xbb67ae85,0x6a09e667));
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx, const BYTE data[], size_t len) {
    WORD i;

    for (i = 0; i < len; ++i) {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        if (ctx->datalen == 64) {
            sha256_transform(ctx, ctx->data);
            ctx->bitlen += 512;
            ctx->datalen = 0;
        }
    }
}

void sha256_final(SHA256_CTX *ctx, BYTE hash[]) {
    WORD i;
    
    // WORD z;
    // WORD j;

    i = ctx->datalen;

    /* Pad whatever data is left in the buffer. */
    if (ctx->datalen < 56) {
        ctx->data[i++] = 0x80;
        while (i < 56)
            ctx->data[i++] = 0x00;
    } else {
        ctx->data[i++] = 0x80;
        while (i < 64)
            ctx->data[i++] = 0x00;
        sha256_transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }

    /* Append to the padding the total message's length in bits and transform. */
    ctx->bitlen += ctx->datalen * 8;
    //You can use SSE to optimize here
    ctx->data[63] = ctx->bitlen;
    ctx->data[62] = ctx->bitlen >> 8;
    ctx->data[61] = ctx->bitlen >> 16;
    ctx->data[60] = ctx->bitlen >> 24;
    ctx->data[59] = ctx->bitlen >> 32;
    ctx->data[58] = ctx->bitlen >> 40;
    ctx->data[57] = ctx->bitlen >> 48;
    ctx->data[56] = ctx->bitlen >> 56;
    sha256_transform(ctx, ctx->data);

    
    /* Since this implementation uses little endian byte ordering and SHA uses big
       endian, reverse all the bytes when copying the final state to the output
       hash. */
    #ifndef MYSSE
     for (i = 0; i < 4; ++i) {
      hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
    } 
    #endif
    #ifdef MYSSE
    WORD k;
    WORD temp[4]={0,0,0,0};
    __m128i majx;
    __m128i gaoqch;
    gaoqch = _mm_sub_epi32(
                _mm_set1_epi32(24),
                _mm_mullo_epi32(
                    _mm_set_epi32(0,1,2,3),
                    _mm_set1_epi32(8)
                )
            );
    // #ifdef DEBUGMODE 
    //     printf("gaoqch= ");
    //     print128_num(gaoqch);
    // #endif //DEBUGMODE
    for(k=0;k<8;++k){
        majx = _mm_and_si128(
                    _mm_srlv_epi32(
                        _mm_set1_epi32((WORD)ctx->state[k]),
                        gaoqch
                    ),
                    _mm_set1_epi32(0x000000ff)
                );
        // #ifdef DEBUGMODE
        //     __m128i my_debug = _mm_srl_epi32(
        //                 _mm_set1_epi32((WORD)ctx->state[k]),
        //                 gaoqch
        //             );
        //     // printf("ctx->state[k] = %x \n",ctx->state[k]);
        //     printf("ctx->state[k] = ");
        //     decToBin(ctx->state[k]);
        //     printf("\n");
        //     printf("my_debug= ");
        //     print128_num(my_debug);

        // #endif//DEBUGMODE

        
        // #ifdef DEBUGMODE
        //     printf("majx= ");
        //     print128_num(majx);
        // #endif
        _mm_store_si128((__m128i*)temp,majx);
        
        // #ifdef DEBUGMODE
        //     printf("temp = %x %x %x %x\n",temp[0], temp[1], temp[2], temp[3]);

        // #endif//DEBUGMODE
        hash[k*4]=temp[3];
        hash[k*4+1]=temp[2];
        hash[k*4+2]=temp[1];
        hash[k*4+3]=temp[0];
    }
    #endif
}