
#include "code.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>

#include "bv8.h"

//  bit vector routines courtesy of prof long...
//

// this was the last file to be updated.

extern uint64_t bytes_read;
extern uint64_t bytes_written;

/*
static uint8_t wrtbuf[BLOCK];
static uint8_t readbuf[BLOCK];
*/

int read_bytes(int infile, uint8_t *buf, int nbytes){
    int total_byte_read = 0;
    while (bytes_read>0){
        bytes_read = read(infile, buf, nbytes);
        if (bytes_read==nbytes) {
            total_byte_read += bytes_read;
            return total_byte_read;  // read all the data requested
        }
        if (bytes_read==0){
            return total_byte_read;     //  That's all folk.  Nothing more to read.  Must be eof
        }
        if (bytes_read < 0){
            printf("There was a problem reading the requeste file\n");
            exit(-1);
        }
        buf += bytes_read;      // Move the buffer pointer ahead by the number of bytes read
        nbytes -= bytes_read;   // subtract the number of bytes_read from the number of bytes requested
        total_byte_read += bytes_read;
    }
    return total_byte_read;
    
}
    
    

int write_bytes(int outfile, uint8_t *buf, int nbytes){
	return write(outfile, buf, nbytes);
}

bool read_bit(int infile, uint8_t *bit){
	infile = infile;
	return true;
}

void write_code(int outfile, Code *c){
/*
	byte_index = 0;
	bitV codev = newVec(c->top);
	for (int i=0; i<c->top; i++) {
		(c,i);
*/
	outfile=outfile;
	return;
}

void flush_codes(int outfile){
	outfile=outfile;
	return;
}

