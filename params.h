#ifndef _PARAMS_H
#define _PARAMS_H

/*
 * Copyright (c) 2018, Henry Corrigan-Gibbs
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <openssl/ec.h>

#ifdef __cplusplus
extern "C"{
#endif

struct params {
  EC_GROUP *group;
  BIGNUM *order;
  BIGNUM *base_prime;
  BN_CTX *ctx;

  EC_POINT *g;
  EC_POINT *h;
};
typedef struct params* Params;
typedef const struct params* const_Params;

typedef enum {
  P256 = 1, 
  P384 = 2, 
  P521 = 3 
} CurveName;

Params Params_new (CurveName c);
void Params_free (Params p);

const EC_GROUP *Params_group (const_Params p);
EC_POINT *Params_point_new (const_Params p);
const BIGNUM *Params_order (const_Params p);
const EC_POINT *Params_gen (const_Params p);
BN_CTX *Params_ctx (const_Params p);
const EC_POINT *Params_g (const_Params p);
const EC_POINT *Params_h (const_Params p);

int Params_rand_point (const_Params p, EC_POINT *point);
int Params_rand_exponent (const_Params p, BIGNUM *x);
int Params_rand_point_exp (const_Params p, EC_POINT *point, BIGNUM *x);

// Compute g.h 
int Params_mul (const_Params p, EC_POINT *res, const EC_POINT *g, const EC_POINT *h);
// Compute g/h
int Params_div (const_Params p, EC_POINT *res, const EC_POINT *g, const EC_POINT *h);
// Compute g^x where g is the fixed generator
int Params_exp (const_Params p, EC_POINT *point, const BIGNUM *exponent);
// Compute h^x for any point h
int Params_exp_base (const_Params p, EC_POINT *point, 
    const EC_POINT *base, const BIGNUM *exponent);
int Params_exp_base2 (const_Params p, EC_POINT *point, 
    const EC_POINT *base1, const BIGNUM *e1,
    const EC_POINT *base2, const BIGNUM *e2);
int Params_exp_base_g (const_Params p, EC_POINT *point,
    const BIGNUM *exp);
int Params_exp_base_h (const_Params p, EC_POINT *point,
    const BIGNUM *exp);

int Params_point_to_exponent (const_Params p, BIGNUM *exp,
                              const EC_POINT *point);

int Params_hash_to_exponent (const_Params p, BIGNUM *exp, 
    const uint8_t *str, int strlen);
int hash_to_bytes (uint8_t *bytes_out, int outlen,
    const uint8_t *bytes_in, int inlen);
int Params_hash_point(const_Params p, EVP_MD_CTX *mdctx, const uint8_t *tag,
               int taglen, const EC_POINT *pt);
int Params_hash_to_point (const_Params p, EC_POINT *point,
    const uint8_t *str, int strlen);


#ifdef __cplusplus
}
#endif
#endif
// #ifndef _PARAMS_H
// #define _PARAMS_H

// #include <openssl/ec.h>

// #ifdef __cplusplus
// extern "C"{
// #endif

// #define IV_LEN 16
// #define TAG_LEN 16
// #define AES128_KEY_LEN 16 

// #define AES256_KEY_LEN 32
// #define AES256_IV_LEN 32

// #define KEY_LEN 32
// #define NUM_LEAVES_HEX_STR "200000"
#define FIELD_ELEM_LEN 32
	
// typedef struct {
//     BIGNUM *base_prime;
//     BIGNUM *numLeaves;
//     BN_CTX *bn_ctx;
//     EC_GROUP *group;
//     BIGNUM *order;
// } Params;

// Params *Params_new();
// void Params_free(Params *params);

void Params_bytesToPoint(Params params, const uint8_t *bytes, EC_POINT *pt);
void Params_pointToBytes(Params params, uint8_t *bytes, const EC_POINT *pt);

// int hash_to_bytes (uint8_t *bytes_out, int outlen,
//     const uint8_t *bytes_in, int inlen);

// int aesEncrypt(const void *key, const uint8_t *pt, int ptLen, uint8_t *iv, uint8_t *ct);
// int aesDecrypt(const void *key, uint8_t *pt, const uint8_t *iv, const uint8_t *ct, int ctLen);

// int intsToBignums(BIGNUM **bns, uint8_t *ints, int len);

// void hmac(uint8_t *key,  uint8_t *out, uint8_t *in, int inLen);

// #ifdef __cplusplus
// }
// #endif
// #endif