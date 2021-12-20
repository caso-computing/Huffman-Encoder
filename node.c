#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

typedef struct Node Node;

struct Node {
    Node *left;
    Node *right;
    uint8_t symbol;
    uint64_t frequency;
};

Node *node_create(uint8_t symbol, uint64_t frequency){
        Node *a_node = (Node *) malloc(sizeof(Node));
        a_node->symbol = symbol;
        a_node->frequency = frequency;
	a_node->left = NULL;
	a_node->right= NULL;
	return a_node;
}

Node *node_join(Node *left, Node *right){
    Node *parent;
    parent = node_create((uint8_t) '$', left->frequency+right->frequency);
    parent->left = left;
    parent->right = right;
    return parent;
}

void node_print(Node *n){

    	fprintf(stderr, "Node symbol: %c, frequency %llu: ", n->symbol, n->frequency);
    	if (n->left != NULL) fprintf(stderr, "left: %c, ", n->left->symbol);
	if (n->right !=NULL) fprintf(stderr, "right: %c", n->right->symbol);
	fprintf(stderr, "\n");
	if (n->left != NULL) node_print(n->left);
	if (n->right != NULL) node_print(n->right);
	return;
}

