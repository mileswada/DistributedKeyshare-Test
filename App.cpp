#include <iostream>
#include "user.cpp"
#include "directory.cpp"
#include "common.h"
#include <bits/stdc++.h>
#include <vector>
#include <stdlib.h>

using namespace std;

int main() {

	vector<User> nominating_committee;

	int t = 2; // Threshold. Unused for now
    int n = 10;

	// Initialize parameters and secret
	Params params = Params_new(P256); 
    BIGNUM *secret =  BN_new();
	BIGNUM *secret_test =  BN_new();
    BN_rand_range(secret, params->order);

	// Initialize Users
	User* users = (User*) malloc(n * sizeof(User));
	for (int i = 0; i < n; i++) {
		users[i] = User("user" + to_string(i));
    }

	// Initialize Key Directory
	Directory new_dict = Directory();
	for (int i = 0; i < n; i++) {
		new_dict.add_public_key(users[i].get_username(), users[i].get_public_key());
    }
	printf("Key Directory created \n");

	// Begin Rounds of Cryptographic Sortition
	int num_rounds = 1;
	for (int i = 0; i < num_rounds; i++) {

		//Generate publicly known random seed
		srand (time(NULL));
		uint8_t *seed = (uint8_t *) malloc(8);
		*seed = rand() % 256;
		nominating_committee.clear();

		// Compute modulus
		BIGNUM *mod = BN_new();
		BIGNUM *one = BN_new();
		BN_one(one);
		BN_set_word(mod, 5);
		for (int j = 0; j < n; j++) {
			users[j].runVRF(seed);

			// Determine if VRF output is sufficient for nominating committee selection
			BIGNUM *remainder = BN_new();
			BN_mod(remainder, users[j].get_vrf_hash(), mod, params->ctx);
			if (BN_cmp(one, remainder) == 1) {
				nominating_committee.push_back(users[j]);
				printf("User %d selected for nominating committee. \n", j);
			}
		}

		// Verify VRF outputs
		for (int j = 0; j < n; j++) {
			EC_POINT *pk = new_dict.get_public_key(users[j].get_username());
			int result = VRF_verify (users[j].get_params(), pk, seed, sizeof seed,
				users[j].get_vrf_hash(), users[j].get_latest_vrf_proof());
			if (result == 1) {
				printf("User %d followed protocol correctly\n", j);
			} else {
				printf("User %d did not follow protocol correctly\n", j);
			}
		}
		
	}
	
	return 0;
}


// int main() {
// 	// Initialize Parameters
// 	int t = 2;
//     int n = 3;
// 	Params params = Params_new(P256); 
//     BIGNUM *secret =  BN_new();
// 	BIGNUM *secret_test =  BN_new();
//     BN_rand_range(secret, params->order);


// 	// Initialize Users
// 	User* users = (User*) malloc(n * sizeof(User));
// 	for (int i = 0; i < n; i++) {
// 		users[i] = User("user" + to_string(i));
// 		cout << "User " << i << " public/private key pair generated successfully" << endl;
//     }
	
//     // Generate secret and secret shares	
// 	ShamirShare *shares[n];
//     ElGamal_ciphertext *encrypted_shares[n];

// 	for (int i = 0; i < n; i++) {
// 		shares[i] = ShamirShare_new();
//     }
// 	Shamir_CreateShares(t, n, secret, params->order, shares, NULL);
//     BIGNUM ** x_s = (BIGNUM **) malloc(n * sizeof(BIGNUM*));
// 	BIGNUM ** y_s = (BIGNUM **) malloc(n * sizeof(BIGNUM*));
// 	for (int i = 0; i < n; i++) {
// 		// x_s[i] = shares[i]->x;
// 		// y_s[i] = shares[i]->y;
// 		x_s[i] = BN_new();
// 		y_s[i] = BN_new();

// 		// BN_rand_range(x_s[i], params->order);
// 		// BN_rand_range(y_s[i], params->order);
// 		BN_copy(x_s[i], shares[i]->x);
// 		BN_copy(y_s[i], shares[i]->y);
//     }

	
// 	ElGamal_ciphertext **enc_x_s = (ElGamal_ciphertext **) malloc(n * sizeof(ElGamal_ciphertext*));
// 	ElGamal_ciphertext **enc_y_s = (ElGamal_ciphertext **) malloc(n * sizeof(ElGamal_ciphertext*));;
    
// 	// Encrypt secret shares using Elgamal and send to each User
// 	for (int i = 0; i < n; i++) {
// 		enc_x_s[i] = ElGamalCiphertext_new(params);
// 		enc_y_s[i] = ElGamalCiphertext_new(params);

// 		BIGNUM *msgTestx = BN_new();
// 		BIGNUM *msgTesty = BN_new();
		
// 		ElGamal_Encrypt(params, x_s[i], users[i].getPublicKey(), NULL, NULL, enc_x_s[i]);
// 		// cout << "Before:" << endl;
// 		// cout << enc_x_s[i]->R << endl;
// 		// cout << enc_x_s[i]->C << endl;
// 		ElGamal_Encrypt(params, y_s[i], users[i].getPublicKey(), NULL, NULL, enc_y_s[i]);
// 		// cout << "After:" << endl;
// 		// cout << enc_x_s[i]->R << endl;
// 		// cout << enc_x_s[i]->C << endl;
// 		// ElGamal_Decrypt(params, msgTestx, users[i].getSecretKey(), enc_x_s[i]);
// 		// ElGamal_Decrypt(params, msgTesty, users[i].getSecretKey(), enc_y_s[i]);
// 		// cout << "x:" << endl;
// 		// cout << BN_bn2hex(x_s[i]) << endl;
// 		// cout << BN_bn2hex(msgTestx) << endl;
// 		// cout << "y:" << endl;
// 		// cout << BN_bn2hex(y_s[i]) << endl;
// 		// cout << BN_bn2hex(msgTesty) << endl;

// 	//	users[i].receiveKeyShare(enc_x_s[i], enc_y_s[i]);
//     }
	
//     // Retrieve secret shares from each User
// 	ShamirShare *retrievedShares[n];
	
// 	for (int i = 0; i < n; i++) {
// 		BIGNUM *decryptedShare_x = BN_new();
//         BIGNUM *decryptedShare_y = BN_new();
//         ElGamal_Decrypt(params, decryptedShare_x, users[i].getSecretKey(), enc_x_s[i]);
//         ElGamal_Decrypt(params, decryptedShare_y, users[i].getSecretKey(), enc_y_s[i]);
// 		// cout << BN_bn2hex(decryptedShare_x) << endl;
// 		// cout << BN_bn2hex(decryptedShare_y) << endl;

// 		retrievedShares[i] = ShamirShare_new();
// 		retrievedShares[i]->x = decryptedShare_x;
// 		retrievedShares[i]->y = decryptedShare_y;
//     }
//     // Reconstruct secret and verify that it's correct
// 	Shamir_ReconstructShares(t, n, retrievedShares, params->order, secret_test);
// 	if (BN_cmp(secret, secret_test) == 0) {
// 		cout << "secret retrieved correctly";
// 	} else {
// 		cout << "secret incorrect" << endl;
// 		char *str = BN_bn2hex(secret);
// 		char *str_test = BN_bn2hex(secret_test);
// 		cout << str << endl;
// 		cout << str_test << endl;
// 	}
	
//     return 0;
// }
