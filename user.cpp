#include <iostream>
#include "shamir.h"
#include "params.h"
#include "elgamal.h"
#include <openssl/ec.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

using namespace std;

// Right now only supports storing one key share at a time
class User {
    public:
        User(string name) {
            cout << "Initializing user " << name << endl;
            // generate pk and sk for user
            Params *user_params = Params_new(); 
            params = user_params;
            sk = BN_new();
            pk = EC_POINT_new(params->group);

            BN_rand_range(sk, params->order);
            EC_POINT_mul(params->group, pk, sk, NULL, NULL, params->bn_ctx);
        }

        EC_POINT* getPublicKey() {
            return pk;
        }

        BIGNUM* getSecretKey() {
            return sk;
        }

        int receiveKeyShare(ElGamal_ciphertext *share_x, ElGamal_ciphertext *share_y) {
            keyShare_x = share_x;
            keyShare_y = share_y;
            /*uint8_t * stored_x = (uint8_t *) malloc(2*FIELD_ELEM_LEN);
            uint8_t * stored_y = (uint8_t *) malloc(2*FIELD_ELEM_LEN);
            ElGamal_Marshal(params, stored_x, keyShare_x);
            ElGamal_Marshal(params, stored_y, keyShare_y);
            cout << *stored_x << endl;
            cout << *stored_y << endl;*/
            return 0;
        };

        ShamirShare* retrieveKeyShare() { 
            BIGNUM *decryptedShare_x = BN_new();
            BIGNUM *decryptedShare_y = BN_new();
            ElGamal_Decrypt(params, decryptedShare_x, sk, keyShare_x);
            ElGamal_Decrypt(params, decryptedShare_y, sk, keyShare_y);
            // char *retrieved_x = BN_bn2hex(decryptedShare_x);
            // char *retrieved_y = BN_bn2hex(decryptedShare_y);
            // cout << retrieved_x << endl;
            // cout << retrieved_y << endl;
            ShamirShare *decryptedShare = ShamirShare_new();
            decryptedShare->x = decryptedShare_x;
            decryptedShare->y = decryptedShare_y;
            return decryptedShare;
        };

        Params* getParams() {
            return params;
        }

    

    private:
        EC_POINT *pk;
        BIGNUM *sk;
        Params *params;
        ElGamal_ciphertext *keyShare_x;
        ElGamal_ciphertext *keyShare_y;

};
