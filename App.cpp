#include <iostream>
#include "User.cpp"
#include <bits/stdc++.h>
#include <vector>
#include <openssl/rsa.h>
#include "CryptoTest.cpp"

using namespace std;

int main() {

	// Initialize Parameters
	int t = 2;
    int n = 3;
	BIGNUM *prime =  BN_new();
    BIGNUM *secret =  BN_new();
	BIGNUM *secret_test =  BN_new();
	BN_hex2bn(&prime, "CC71BAE525F36E3D3EB843232F9101BD");
    BN_rand_range(secret, prime);
   
	// Initialize Users
	User* users = (User*) malloc(n * sizeof(User));
	for (int i = 0; i < n; i++) {
		users[i] = User("user" + to_string(i));
    }
	
    // Generate secret and secret shares	
	ShamirShare *shares[n];
    ShamirShare *sharesOut[2 * t];
	for (int i = 0; i < n; i++) {
		shares[i] = ShamirShare_new();
    }
	Shamir_CreateShares(t, n, secret, prime, shares, NULL);
    
    // Send secret shares to each User
	for (int i = 0; i < n; i++) {
		users[i].receiveKeyShare(shares[i]);
    }

    // Retrieve secret shares from each User
	ShamirShare *retrievedShares[n];
	for (int i = 0; i < n; i++) {
		retrievedShares[i] = users[i].retrieveKeyShare();
    }
    // Reconstruct secret and verify that it's correct
	Shamir_ReconstructShares(t, n, retrievedShares, prime, secret_test);
	if (BN_cmp(secret, secret_test) == 0) {
		cout << "secret retrieved correctly";
	} else {
		cout << "secret incorrect";
		// cout << *secret << endl;
		// cout << *secret_test << endl;
	}
    return 0;
}

