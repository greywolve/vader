/*
 Authors : Zico da Silva, Nirav Domah, Oliver Powell, Ayeshah Bharoochi.
 
 Date : 16 May 2015
 
 Program to compute hash values of passwords, and to send them
 to a serial port to be transported to an FPGA via USB. 
 
 To use this code, run through the terminal and follow the easy steps.
 
 To compile:
        $gcc Vader.c -o vader -lcrypto
 To run:
        $./vader
 
*/

#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/md5.h>

#define noOfPasswords 3106

void read_passwords(char* file_name);
void write_passwords(char* file_name);
int set_interface_attribs (int fd, int speed, int parity);
void set_blocking (int fd, int should_block);

unsigned char hashTable[noOfPasswords][MD5_DIGEST_LENGTH];	// hash table stores hashed passwords; MD5_DIGEST_LENGTH = 16
char passwordTable[noOfPasswords][255];

int main(void){
    
    
    //perform hashing.
    char *infile_name = "john.txt";
    char *outfile_name = "HashTable.txt";
    

    
    read_passwords(infile_name);
    
    // perform hashing and store in hashTable
    int i;
    for(i = 0; i < noOfPasswords; i++)
    {
        MD5(passwordTable[i], strlen(passwordTable[i]), hashTable[i]);
    }
    
    write_passwords(outfile_name);
    
    //send to serial port.
    
//    char *portname = "/dev/ttyUSB1";
//    int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
//    
//    if (fd < 0){
//        printf ("error %d opening %s: %s", errno, portname, strerror (errno));
//        return 1;
//    }
//    
//    set_interface_attribs (fd, B9600, 0);  		// set speed to 115,200 bps, 8n1 (no parity)
//    set_blocking (fd, 0);                		// set no blocking
//    
//    write (fd, "hello Nirav", 11);          		// send 7 character greeting
//    
//    usleep ((7 + 25) * 10000);             		// sleep enough to transmit the 7 plus
//    // receive 25:  approx 100 uS per char transmit
//    char buf [100];
//    buf[0] = NULL;
//    int n = read (fd, buf, sizeof buf);  		// read up to 100 characters if ready to read
//    buf[n] = NULL;
//    
//    printf("Buffer content: (%d) %s\n",n,buf);
    
    return 0;
}

// read passwords from text file into passwordTable
void read_passwords(char* file_name){

    FILE *inputFile;
    inputFile = fopen(file_name, "r");
    int i;
    for(i = 0; i < noOfPasswords; i++)
    {
        fscanf(inputFile, "%s", passwordTable[i]);
    }
    fclose(inputFile);
}


// save hashed passwords to text file
void write_passwords(char* file_name){

    FILE *outputFile;
    outputFile = fopen(file_name, "w+");
    int j;
    int i;
    for(j = 0; j < noOfPasswords; j++)
    {
        // NOTE: hashTable[j] is a  sequence of 16 hex  numbers, each having a max value of ff (1 byte)
        
        // send hashTable[j] to FPGA here	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        
        // output hash to console
        for(i = 0; i < MD5_DIGEST_LENGTH; i++)
            fprintf(outputFile, "%x", hashTable[j][i]);
        fprintf(outputFile, "\n");
    }
    fclose(outputFile);
}



int set_interface_attribs (int fd, int speed, int parity)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0)
    {
        printf ("error %d from tcgetattr", errno);
        return -1;
    }
    
    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);
    
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
    
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
    
    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    
    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        printf ("error %d from tcsetattr", errno);
        return -1;
    }
    return 0;
}

void set_blocking (int fd, int should_block)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0)
    {
        printf ("error %d from tggetattr", errno);
        return;
    }
    
    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
    
    if (tcsetattr (fd, TCSANOW, &tty) != 0)
        printf ("error %d setting term attributes", errno);
}


