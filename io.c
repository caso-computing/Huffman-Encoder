
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
	return read(infile, buf, nbytes);
}

int write_bytes(int outfile, uint8_t *buf, int nbytes){
	return write(outfile, buf, nbytes);
}
/******************************************
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
*/
