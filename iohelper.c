#include "code.h"
#include "io.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


static uint8_t outbuffer[BLOCK];
static int outindex=0;

// flushbuffer writes and remaining bytes to be written to outfile and then closes the file.
//

void flush_outbuffer(int outfile){
	if (outindex>0 & outindex < BLOCK) {
		write(outfile, outbuffer, outindex);
                close(outfile);
         }
}

// buffer the bytes to be written until you have a full BLOCK to write.
// make sure you flush the output when you are done writing blocks to ensure everything
// is written to the disk.

bool write_byte(int outfile, uint8_t x){

	//static int end = -1;  //tracks the index after the last valid byte

	//empty buffer if full
	outbuffer[outindex] = x;
	outindex += 1;

	if (outindex == 0) {
		int bytes = write(outfile, outbuffer, BLOCK); // should use write_bytes()
		if (bytes < BLOCK){
			fprintf(stderr, "not all bytes were written to outfile");
			exit(-1);
		}
		outindex = 0;  //reset index to start of buffer since we just dumped a BLOCK
	}
	return true;
}

