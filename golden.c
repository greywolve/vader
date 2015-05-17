// Golden standard for VADER, single threaded CPU version
// compile and run with gcc golden.c -lssl -lcrypto && ./a.out 5
// where 5 is the number of password chars you want

#include <stdlib.h>
#include <stdio.h>
#include <openssl/md5.h>
#include <string.h>
#include <time.h>

// get the next char (used to generate all combinations)
int inc(char *c){
    if(c[0]==0) return 0;
    if(c[0]=='Z'){ //end char
      c[0]='0'; // start char
      return inc(c+sizeof(char));
    }
    c[0]++;
    return 1;
}

// generate a random string
void rand_str(char *dest, size_t length) {
  char charset[] = "0123456789"
                   ":;<=>?@"
                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

int main(int argc, char *argv[]){
    if ( argc != 2 ) {
        printf( "usage: %s <number of chars in password>", argv[0]);
    }
    else {
        int n = atoi(argv[1]); // password length

        char password[n];
        unsigned char password_hash[MD5_DIGEST_LENGTH];
        unsigned char bruteforce_hash[MD5_DIGEST_LENGTH];

        int i,j;
        char *c = malloc((n+1)*sizeof(char));

        int found = 0; // flag

        clock_t begin, end;
        double time_spent;

        // seed random number generator
        srand(time(NULL));
        // generate a random password with specified length
        rand_str(password,n);
        printf("Generated random password: %s\n",password);

        // hash the password
        MD5(password, strlen(password), password_hash);

        printf("Starting password recovery...\n");

        // start timing
        begin = clock();

        // main loop to go through all combinations of passwords in the start and ending char range specified
        for(i=1;i<=n;i++){
          if (!found) {
            for(j=0;j<i;j++) c[j]='0'; // starting char
            c[i]=0;
            do {
                MD5(c, strlen(c), bruteforce_hash);
                // check if the password hash and bruteforce hash are equal
                // if they are equal we have a match and found = 1
                found = !memcmp(password_hash, bruteforce_hash, MD5_DIGEST_LENGTH);
                if (found)
                  printf("Found a match for: %s\n",c);
            } while(inc(c) && !found);
          }
        }

        if (!found)
          printf("No match found for: %s\n",password);

        // stop timing
        end = clock();

        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Time taken: %f ms\n",time_spent * 1000);

        free(c);
    }

    return 0;
}
