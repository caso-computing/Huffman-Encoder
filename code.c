#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

typedef struct {
    uint32_t top;
    uint8_t bits[MAX_CODE_SIZE];
} Code;

Code code_init(void){
	Code c;
	c.top = 0;
	for (int i=0; i<MAX_CODE_SIZE; i++){
		c.bits[i]=0;
	}
	return c;
}


uint32_t code_size(Code *c){
	return c->top;
}

bool code_empty(Code *c) {
	return (bool) c->top;
}

bool code_full(Code *c){
	return (bool) (c->top == ALPHABET);
}

bool code_set_bit(Code *c, uint32_t i){
	if (i>ALPHABET) return false;	//we only have 256 bits to set!
	c->bits[i/MAX_CODE_SIZE] |=  ((uint8_t) 0x1 << i % MAX_CODE_SIZE);
	return true;
}

bool code_clr_bit(Code *c, uint32_t i){
	if (i>ALPHABET) return false;  //come on, only 256 bits!
	c->bits[i/MAX_CODE_SIZE] &= ~((uint8_t) 0x1 << i % MAX_CODE_SIZE);
	return true;
}


bool code_get_bit(Code *c, uint32_t i){
	if (i>ALPHABET) return false;  // please watch your bits
	return ((c->bits[i/MAX_CODE_SIZE]  >> i%MAX_CODE_SIZE) & 0x1);
}


// Push a bit onto the c vector stack.  
// value of bit has to be either 1 or 0.  
//
bool code_push_bit(Code *c, uint8_t bit){
	if (code_full(c)) return false;
	
	if (bit) {
		code_set_bit(c,c->top);
	}
	c->top += 1;
	return true;
}



bool code_pop_bit(Code *c, uint8_t *bit) {
	if (code_empty(c)) return false;
	c->top -= 1;		// get bit just below top
	*bit = code_get_bit(c,c->top);
	code_clr_bit(c,c->top);			//not necessary but helps in reading c
	return bit;
}

void code_print(Code *c){
	for (int i=0;i<c->top;i+=8){
		printf("%x",c->bits[i/MAX_CODE_SIZE]);
	}
}
