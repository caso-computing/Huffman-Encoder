/*
   
   */

#define BLOCK 4096

bool read_bit(int infile, uint8_t *x){
	static uint8_t buffer[BLOCK];
	static int index = 0;
	static int end = -1;  // Tracks the index after the last valid byte.

	// Fill buffer if empty.
	if (index == 0) {
		int bytes = read(infile, buffer, BLOCK);
		if (bytes < BLOCK) {
			// Set the end as 1 after the last valid byte.
			end = (bytes*8) +1;
		}
	}

	//  Return a bit out of the buffer.
	*x = get_bit[buffer];
	index += 1;

	// Reset the index if it reaches the end of the buffer
	//check when to reset the index
	If (index == BLOCK*8) {
		index = 0;
	}

	// Return true/false depending on if there is a valid byte to return.
	return index != end;
}

// write_code and flush_codes share a buffer.
//

static u8 buffer[BLOCK];
static int index = 0;  // tracks position in buffer

void write_code(int outfile, Code *c){
	// buffers a code
	// buffers are written when filled
	// used by encoder

	// buffer each bit into the buffer
	for (i=0; i<code_size(); i+=1){
		bit = code_get_bit(c,i);
		if (bit == 1) {
			//set the bit at index
		}
		else {
			//clear the bit at index
		}	}
		// if buffer is filled, flush buffer{

			flush_codes(outfile);
		}


void flush_codes(int outfile) {
	// flush bits out of buffer if not empty.
	if (index > 0) {
		// covert index into bytes
		write_bytes(outfile...);
	}

}

//  make sure you write enough bytes to cover for example 18 bits.  18%8? or 18/8 (floor)


