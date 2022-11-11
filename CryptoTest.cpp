#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/rand.h>
#include "params.h"
#include "common.h"
#include "shamir.h"
#include "elgamal.h"


void ShamirTest() {
    int rv = ERROR;
    int t = 2;
    int n = 6;
    BIGNUM *prime = NULL;
    BIGNUM *secret = NULL;
    BIGNUM *secret_test = NULL;
    ShamirShare *shares[n];
    ShamirShare *sharesOut[2 * t];
    uint8_t order[2 * t];
    // Params *params;

    printf("----- SHAMIR SECRET SHARING TEST -----\n");

    CHECK_A (prime = BN_new());
    CHECK_A (secret = BN_new());
    CHECK_A (secret_test = BN_new());
    // CHECK_A (params = Params_new());
    
    for (int i = 0; i < n; i++) {
        CHECK_A (shares[i] = ShamirShare_new());
    }

    BN_hex2bn(&prime, "CC71BAE525F36E3D3EB843232F9101BD");
//    BN_hex2bn(&prime, "EC35D1D9CD0BEC4A13186ED1DDFE0CF3");
    CHECK_C (BN_rand_range(secret, prime));

    CHECK_C (Shamir_CreateShares(t, n, secret, prime, shares, NULL));
    CHECK_C (Shamir_ValidateShares(t, n, shares, prime));
    CHECK_C (Shamir_ReconstructShares(t, n, shares, prime, secret_test));
    if (BN_cmp(secret, secret_test) != 0) {
        printf("Shamir secret sharing FAILED\n");
        printf("secret: %s\n", BN_bn2hex(secret));
        printf("reconstructed secret: %s\n", BN_bn2hex(secret_test));
    } else {
        printf("Shamir secret sharing successful: %s\n", BN_bn2hex(secret_test));
    }
    
    CHECK_C (Shamir_ReconstructSharesWithValidation(t, n, shares, prime, secret_test));
    //printf("before find valid shares\n");
    CHECK_C (Shamir_FindValidShares(t, n, shares, sharesOut, order, prime, secret_test));
    printf("after find valid shares\n");
    printf("order of valid shares: "); 
    for (int i = 0; i < 2 * t; i++) {
        printf("%d ", order[i]);
    }
    printf("\n");

    CHECK_C (Shamir_CreateShares(1, 2, secret, prime, shares, NULL));
    CHECK_C (Shamir_ValidateShares(t, n, shares, prime) == ERROR);

    if (BN_cmp(secret, secret_test) != 0) {
        printf("Shamir secret sharing FAILED\n");
        printf("secret: %s\n", BN_bn2hex(secret));
        printf("reconstructed secret: %s\n", BN_bn2hex(secret_test));
    } else {
        printf("Shamir secret sharing successful.\n");
    }

cleanup:
    if (rv == ERROR) printf("FAILED Shamir secret sharing tests\n");
    BN_free(prime);
    BN_free(secret);
    BN_free(secret_test);
    for (int i = 0; i < n; i++) {
        ShamirShare_free(shares[i]);
    }
}

void ElGamalTest() {
    printf("----- EL GAMAL TEST -----\n");
    Params *params = Params_new();
    BIGNUM *sk = BN_new();
    EC_POINT *pk = EC_POINT_new(params->group);
    ElGamal_ciphertext *c1 = ElGamalCiphertext_new(params);
    ElGamal_ciphertext *c2 = ElGamalCiphertext_new(params);
    BIGNUM *msg1 = BN_new();
    BIGNUM *msgTest1 = BN_new();

    BIGNUM *msg2 = BN_new();
    BIGNUM *msgTest2 = BN_new();

    BN_rand_range(sk, params->order);
    BN_rand_range(msg1, params->order);
    BN_rand_range(msg2, params->order);
    EC_POINT_mul(params->group, pk, sk, NULL, NULL, params->bn_ctx);

    // BIGNUM *r = BN_new();
    // EC_POINT *R = EC_POINT_new(params->group);
    // BN_rand_range(r, params->order);
    // EC_POINT_mul(params->group, R, r, NULL, NULL, params->bn_ctx);

    ElGamal_Encrypt(params, msg1, pk, NULL, NULL, c1);
    ElGamal_Encrypt(params, msg2, pk, NULL, NULL, c2);
    
    uint8_t bytes[66];
    ElGamal_Marshal(params, bytes, c1);

    uint8_t bytes2[66];
    ElGamal_Marshal(params, bytes2, c2);

    ElGamal_Decrypt(params, msgTest1, sk, c1);
    ElGamal_Decrypt(params, msgTest2, sk, c2);

    printf("%s\n", "message 1:");
    printf("%s\n", BN_bn2hex(msg1));
    printf("%s\n", BN_bn2hex(msgTest1));
    printf("%s\n", "message 2:");
    printf("%s\n", BN_bn2hex(msg2));
    printf("%s\n", BN_bn2hex(msgTest2));
}

int main() {

    ElGamalTest();
    return 0;
}