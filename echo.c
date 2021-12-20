//  wrapper io function to read bytes from a file in nbyte chunks.
//  assumes nbytes < buf size
//

/*
bool read_byte(int infile, u8 *x)
	//reads a block of bytes into a buffer
	//returns 1 byte at a time from that buffer.
	//returns true if there are bits, false otherwise.
	// if all the bits in the buffer have been returned, repeat process


int read_bytes(int infile, uint8_t *buf, int nbytes) {
	
	static u8 buf[BLOCK]
	static int index =0;  // tracks position in buffer
	static int end = -1;  // tracks the last valid byte
	
	bytes_read = read(infile, buf, nbytes);

//Fill buffer if empty.

	if (index ==0) {
		bytes = read_bytes (infd, buf, BLOCK);
		if (bytes < BLOCK) {  //reached end of file
			// set the end.
			// end is marked as 1 after the last valid byte
			end = bytes+1
		}
	}
// return a byte out of buffer

	*x=buffer[index];
	index +=1;
	if (index == BLOCK){
		index = 0;
	}
// return true/false
	return index != end;

*/

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdbool.h>
	
#define BLOCK 4096

bool read_byte(int infile, uint8_t *x){
	static uint8_t buffer[BLOCK];
	static int index = 0;
	static int end = -1;  // Tracks the index after the last valid byte.

	// Fill buffer if empty.
	if (index == 0) {
		int bytes = read(infile, buffer, BLOCK);  //  should use read_bytes() here
		if (bytes < BLOCK) {
			// Set the end as 1 after the last valid byte.
			end = bytes +1;
		}
	}

	//  Return a byte out of the buffer.
	*x = buffer[index];
	index += 1;

	// Reset the index if it reaches the end of the buffer
	if (index == BLOCK) {
		index = 0;
	}

	// Return true/false depending on if there is a valid byte to return.
	return index != end;
}

int main(int argc, char **argv) {
	uint8_t byte;
	uint8_t nl = '\n';

	while (read_byte(STDIN_FILENO, &byte )==true){
		write(STDOUT_FILENO, &byte, 1);
		write(STDOUT_FILENO, &nl, 1);
	}
	return 0;
}
