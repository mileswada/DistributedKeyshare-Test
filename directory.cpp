#include <iostream>
#include <map>
#include <openssl/ec.h>


using namespace std;

class Directory {
    public:
        Directory() {
            cout << "Initializing Directory" << endl;
        }
        
        int add_public_key(string username, EC_POINT* pk) {
            directory[username] = pk;
            return 0;
        }

        EC_POINT* get_public_key(string username) {
            if (directory.find(username) != directory.end()) {
                return directory[username];
            }
            return NULL;
        }

    private:
        map<string, EC_POINT*> directory;

};