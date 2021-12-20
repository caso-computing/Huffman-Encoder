#include "node.h"
#include "code.h"
#include "pq.h"
#include "iohelper.h"
#include "io.h"

#include "defines.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


Node *build_tree(uint64_t hist[static ALPHABET]){
	PriorityQueue *pq = pq_create(ALPHABET);
	Node *left;
	Node *right;
	Node *parent;
	for (int sym=0; sym<ALPHABET; sym++){
		if (hist[sym] != 0){
			Node *n = node_create(sym,hist[sym]);
			if (!enqueue(pq,n)) exit(-1);   	//Oops.  Houston we have a problem with a full queue.
		}
	}
	pq_print(pq);
	while (pq_size(pq) > 1) {
		dequeue(pq, &left);
		dequeue(pq, &right);
		parent = node_join(left, right);
		enqueue(pq, parent);
	}
	node_print(parent);
	
	return parent;
}

void build_codes(Node *root, Code table[static ALPHABET]) {
	static Code c;
	static uint8_t bitbucket;

	if (root != NULL) {
		if (!root->left & !root->right){
			table[root->symbol] = c;
			fprintf(stderr, "%3d: ",root->symbol);
		}
		else {
			code_push_bit(&c,0);
			build_codes(root->left, table);
			code_pop_bit(&c,&bitbucket);

			code_push_bit(&c,1);
			build_codes(root->right, table);
			code_pop_bit(&c,&bitbucket);
		}
	}
}



void dump_tree(int outfile, Node *root) {
	if (root != NULL){
		dump_tree(outfile, root->left);
		dump_tree(outfile, root->right);

		if ((!root->left) && (!root->right)) {
			//we have a leaf node
			write_byte(outfile,'L');
			write_byte(outfile, root->symbol);
			fprintf(stderr, "L");
			fprintf(stderr, "%d",root->symbol);
		}

		else {
			//interior node
			write_byte(outfile,'I');
			fprintf(stderr, "I");
		}
	}

	return;
}


Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
	Node *root;
	return root;
}


void delete_tree(Node **root) {
	return;
}

