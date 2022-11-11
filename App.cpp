#include <iostream>
#include "user.cpp"
#include "common.h"
#include <bits/stdc++.h>
#include <vector>

using namespace std;


int main() {
	// Initialize Parameters
	int t = 2;
    int n = 3;
	Params *params = Params_new(); 
    BIGNUM *secret =  BN_new();
	BIGNUM *secret_test =  BN_new();
    BN_rand_range(secret, params->order);


	// Initialize Users
	User* users = (User*) malloc(n * sizeof(User));
	for (int i = 0; i < n; i++) {
		users[i] = User("user" + to_string(i));
		cout << "User " << i << " public/private key pair generated successfully" << endl;
    }
	
    // Generate secret and secret shares	
	ShamirShare *shares[n];
    ElGamal_ciphertext *encrypted_shares[n];

	for (int i = 0; i < n; i++) {
		shares[i] = ShamirShare_new();
    }
	Shamir_CreateShares(t, n, secret, params->order, shares, NULL);
    BIGNUM ** x_s = (BIGNUM **) malloc(n * sizeof(BIGNUM*));
	BIGNUM ** y_s = (BIGNUM **) malloc(n * sizeof(BIGNUM*));
	for (int i = 0; i < n; i++) {
		// x_s[i] = shares[i]->x;
		// y_s[i] = shares[i]->y;
		x_s[i] = BN_new();
		y_s[i] = BN_new();

		// BN_rand_range(x_s[i], params->order);
		// BN_rand_range(y_s[i], params->order);
		BN_copy(x_s[i], shares[i]->x);
		BN_copy(y_s[i], shares[i]->y);
    }

	
	ElGamal_ciphertext **enc_x_s = (ElGamal_ciphertext **) malloc(n * sizeof(ElGamal_ciphertext*));
	ElGamal_ciphertext **enc_y_s = (ElGamal_ciphertext **) malloc(n * sizeof(ElGamal_ciphertext*));;
    
	// Encrypt secret shares using Elgamal and send to each User
	for (int i = 0; i < n; i++) {
		enc_x_s[i] = ElGamalCiphertext_new(params);
		enc_y_s[i] = ElGamalCiphertext_new(params);

		BIGNUM *msgTestx = BN_new();
		BIGNUM *msgTesty = BN_new();
		
		ElGamal_Encrypt(params, x_s[i], users[i].getPublicKey(), NULL, NULL, enc_x_s[i]);
		// cout << "Before:" << endl;
		// cout << enc_x_s[i]->R << endl;
		// cout << enc_x_s[i]->C << endl;
		ElGamal_Encrypt(params, y_s[i], users[i].getPublicKey(), NULL, NULL, enc_y_s[i]);
		// cout << "After:" << endl;
		// cout << enc_x_s[i]->R << endl;
		// cout << enc_x_s[i]->C << endl;
		// ElGamal_Decrypt(params, msgTestx, users[i].getSecretKey(), enc_x_s[i]);
		// ElGamal_Decrypt(params, msgTesty, users[i].getSecretKey(), enc_y_s[i]);
		// cout << "x:" << endl;
		// cout << BN_bn2hex(x_s[i]) << endl;
		// cout << BN_bn2hex(msgTestx) << endl;
		// cout << "y:" << endl;
		// cout << BN_bn2hex(y_s[i]) << endl;
		// cout << BN_bn2hex(msgTesty) << endl;

	//	users[i].receiveKeyShare(enc_x_s[i], enc_y_s[i]);
    }
	
    // Retrieve secret shares from each User
	ShamirShare *retrievedShares[n];
	
	for (int i = 0; i < n; i++) {
		BIGNUM *decryptedShare_x = BN_new();
        BIGNUM *decryptedShare_y = BN_new();
        ElGamal_Decrypt(params, decryptedShare_x, users[i].getSecretKey(), enc_x_s[i]);
        ElGamal_Decrypt(params, decryptedShare_y, users[i].getSecretKey(), enc_y_s[i]);
		// cout << BN_bn2hex(decryptedShare_x) << endl;
		// cout << BN_bn2hex(decryptedShare_y) << endl;

		retrievedShares[i] = ShamirShare_new();
		retrievedShares[i]->x = decryptedShare_x;
		retrievedShares[i]->y = decryptedShare_y;
    }
    // Reconstruct secret and verify that it's correct
	Shamir_ReconstructShares(t, n, retrievedShares, params->order, secret_test);
	if (BN_cmp(secret, secret_test) == 0) {
		cout << "secret retrieved correctly";
	} else {
		cout << "secret incorrect" << endl;
		char *str = BN_bn2hex(secret);
		char *str_test = BN_bn2hex(secret_test);
		cout << str << endl;
		cout << str_test << endl;
	}
	
    return 0;
}