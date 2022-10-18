#include <iostream>
#include "shamir.h"
using namespace std;

// Right now only supports storing one key share at a time
class User {
    public:
        User(string name) {
            cout << "Initializing user " << name << endl;

            // Setup public private key pair
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
        int publicKey;
        int privateKey;
        ShamirShare *keyShare;

};