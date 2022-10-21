#include <iostream>
#include "shamir.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

using namespace std;

// Right now only supports storing one key share at a time
class User {
    public:
        User(string name) {
            cout << "Initializing user " << name << endl;

            // Setup public private key pair
            // pRSA = RSA_generate_key(2048, 3, NULL, NULL);
            
            // BIGNUM *n = BN_new();
            // BIGNUM *e = BN_new();
            // BIGNUM *d = BN_new();
            // RSA_get0_key(pRSA, &n, &e, &d);
        }

        int getPublicKey() {
            return 0;
        }

        int receiveKeyShare(ShamirShare *share) {
            keyShare = share;
            return 0;
        };

        ShamirShare* retrieveKeyShare() {
            return keyShare;
        };

    

    private:
        RSA *pRSA;

        ShamirShare *keyShare;

};
