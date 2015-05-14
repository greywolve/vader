/*
 * Creates a hash table using the MD5 hash function.
 * If more passwords are added to the passwordTable, update noOfPasswords.
 * 
 * Each entry of hashTable is 16 byte wide and contains 16 8bit hex numbers (total of 16 bytes or 16 chars).
 * Send hashTable entries to FPGA on line 64
 * 
 * To compile:
 * 		$ gcc HashMaker.c -o HashMaker -lcrypto
 * To run:
 * 		$ ./HashMaker
 */


#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

int main()
{
	int i, j;
	int noOfPasswords = 25;
	unsigned char hashTable[noOfPasswords][MD5_DIGEST_LENGTH];	// hash table stores hashed passwords; MD5_DIGEST_LENGTH = 16
	char *passwordTable[] = {
		"hello world",		// should give 5eb63bbbe01eeed093cb22bb8f5acdc3
		"password",
		"12345",
		"12345678",
		"qwerty",
		"123456789",
		"1234",
		"baseball",
		"dragon",
		"football",
		"1234567",
		"monkey",
		"letmein",
		"abc123",
		"111111",
		"mustang",
		"access",
		"shadow",
		"master",
		"michael",
		"superman",
		"696969",
		"123123",
		"batman",
		"trustno1"
	};

	// perform hashing and store in hashTable
	for(i = 0; i < noOfPasswords; i++)
	{
		MD5(passwordTable[i], strlen(passwordTable[i]), hashTable[i]);
	}

	// output
	for(j = 0; j < noOfPasswords; j++)
	{
		// NOTE: hashTable[j] is a  sequence of 16 hex  numbers, each having a max value of ff (1 byte)
		
		// send hashTable[j] to FPGA here	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		
		// output hash to console
		for(i = 0; i < MD5_DIGEST_LENGTH; i++)
			printf("%x ", hashTable[j][i]);
		printf("\n");
	}

	return 0;
}

