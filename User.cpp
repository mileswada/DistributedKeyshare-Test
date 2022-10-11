#include <iostream>
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

        int receiveKeyShare() {
            return 0;
        };

        int retrieveKeyShare() {
            return 0;
        };

    

    private:
        int publicKey;
        int privateKey;
        int keyShare;

};