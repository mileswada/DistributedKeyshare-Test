#include <stdlib.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/evp.h>
#include <string.h>

#include "common.h"
#include "elgamal.h"
#include "params.h"

/* Hashed-ElGamal encryption scheme. */

ElGamal_ciphertext *ElGamalCiphertext_new(Params params) {
    int rv;
    ElGamal_ciphertext *c;

    CHECK_A (c = (ElGamal_ciphertext *)malloc(sizeof(ElGamal_ciphertext)));
    CHECK_A (c->R = EC_POINT_new(params->group));
    CHECK_A (c->C = (uint8_t *)malloc(FIELD_ELEM_LEN));
    
cleanup:
    if (rv == ERROR) {
        ElGamalCiphertext_free(c);
        return NULL;
    }
    return c;
}

void ElGamalCiphertext_free(ElGamal_ciphertext *c) {
    if (c && c->R) EC_POINT_free(c->R);
    if (c && c->C) free(c->C);
    if (c) free(c);
}

/* 66 bytes */
void ElGamal_Marshal(Params params, uint8_t *bytes, ElGamal_ciphertext *c) {
    Params_pointToBytes(params, bytes, c->R);
    memcpy(bytes + 33, c->C, FIELD_ELEM_LEN);
}

/* 66 bytes */
void ElGamal_Unmarshal(Params params, uint8_t *bytes, ElGamal_ciphertext *c) {
    Params_bytesToPoint(params, bytes, c->R);
    memcpy(c->C, bytes + 33, FIELD_ELEM_LEN);
}

int ElGamal_Encrypt(Params params, BIGNUM *msg, EC_POINT *pk, BIGNUM *r, EC_POINT *R, ElGamal_ciphertext *c) {
    int rv;
    EC_POINT *tmp;
    uint8_t pointBuf[33];
    uint8_t keyBuf[33];
    uint8_t hashedKeyBuf[32];
    EC_POINT *hashedTmp;
    uint8_t msgBuf[FIELD_ELEM_LEN];
    EVP_CIPHER_CTX *ctx;
    EVP_MD_CTX *mdctx;
    int bytesFilled = 0;

    if (r == NULL) {
        CHECK_A (r = BN_new());
        CHECK_C (BN_rand_range(r, params->order));
        CHECK_A (R = EC_POINT_new(params->group));
        CHECK_C (EC_POINT_mul(params->group, R, r, NULL, NULL, params->ctx));
    }

    CHECK_A (tmp = EC_POINT_new(params->group));
    CHECK_A (hashedTmp = EC_POINT_new(params->group));

    memset(msgBuf, 0, FIELD_ELEM_LEN);
    BN_bn2bin(msg, msgBuf + FIELD_ELEM_LEN - BN_num_bytes(msg));

    c->R = EC_POINT_dup(R, params->group);

    // pk^r
    CHECK_C (EC_POINT_mul(params->group, tmp, NULL, pk, r, params->ctx));
    // H(pk^r)
    Params_pointToBytes(params, pointBuf, tmp);
 
    CHECK_A (mdctx = EVP_MD_CTX_create());
    CHECK_C (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL));
    CHECK_C (EVP_DigestUpdate(mdctx, pointBuf, 33));
    CHECK_C (EVP_DigestFinal_ex(mdctx, hashedKeyBuf, NULL));

    CHECK_A (ctx = EVP_CIPHER_CTX_new());
    CHECK_C (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, hashedKeyBuf, NULL));
    CHECK_C (EVP_EncryptUpdate(ctx, c->C, &bytesFilled, msgBuf, FIELD_ELEM_LEN));

cleanup:
    return rv;
}

int ElGamal_Decrypt(Params params, BIGNUM *msg, BIGNUM *sk, ElGamal_ciphertext *c) {
    int rv;
    EC_POINT *tmp;
    uint8_t pointBuf[33];
    uint8_t keyBuf[33];
    uint8_t hashedKeyBuf[32];
    EC_POINT *hashedTmp;
    EVP_CIPHER_CTX *ctx;
    EVP_MD_CTX *mdctx;
    int bytesFilled = 0;
    uint8_t msgBuf[FIELD_ELEM_LEN];

    CHECK_A (tmp = EC_POINT_new(params->group));
    CHECK_A (hashedTmp = EC_POINT_new(params->group));

    CHECK_C (EC_POINT_mul(params->group, tmp, NULL, c->R, sk, params->ctx));
    // H(R^sk)
    Params_pointToBytes(params, pointBuf, tmp);
    
    CHECK_A (mdctx = EVP_MD_CTX_create());
    CHECK_C (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL));
    CHECK_C (EVP_DigestUpdate(mdctx, pointBuf, 33));
    CHECK_C (EVP_DigestFinal_ex(mdctx, hashedKeyBuf, NULL));

    CHECK_A (ctx = EVP_CIPHER_CTX_new());
    CHECK_C (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, hashedKeyBuf, NULL));
    CHECK_C (EVP_DecryptUpdate(ctx, msgBuf, &bytesFilled, c->C, FIELD_ELEM_LEN));

    BN_bin2bn(msgBuf, FIELD_ELEM_LEN, msg);

cleanup:
    if (tmp) EC_POINT_free(tmp);
    if (hashedTmp) EC_POINT_free(hashedTmp);
}