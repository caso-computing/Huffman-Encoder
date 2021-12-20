#include "node.h"
#include "pq.h"
#include "heap.h"
#include "stats.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

typedef struct PriorityQueue {
	uint32_t head;
	uint32_t tail;
	uint32_t size;
	Node **items;
} PriorityQueue;


PriorityQueue *pq_create(uint32_t capacity){
	PriorityQueue *a_queue = (PriorityQueue *) malloc(sizeof(PriorityQueue));
	if (a_queue){
		a_queue->head = 0;
		a_queue->tail = 0;  //nothing in queue yet
		a_queue->size = capacity+1;  //  add in extra slot to represent full queue
		a_queue->items = (Node **) calloc(capacity, sizeof(Node *));
		if (!a_queue->items){
			free (a_queue);
			a_queue = NULL;
		}
	}
	return a_queue;
}

void pq_delete(PriorityQueue **q){
	if (*q) {
	//	free((*q->items));
		free(*q);
		q=NULL;
	}
	return;
}


bool pq_empty(PriorityQueue *q){
	if (q) {
		return q->head == q->tail;
	}
	return true;
}


bool pq_full(PriorityQueue *q){
	if (q) {
		return (((q->head+1)%q->size) == q->tail);
	}
	return true;
}


uint32_t pq_size(PriorityQueue *q){
	if (q->tail<q->head){
		return q->head-q->tail;
	}
	else if (q->tail==q->head) {
		return 0;
	}
	return ((q->head+q->size)-q->tail);
}

bool enqueue(PriorityQueue *q, Node *n){
	Stats stats; 		//  Pointer to a statistics structure frm asgn3
	
	if (q) {
		if (pq_full(q)) {
			return false;
			}
		q->items[q->head] = n;
		q->head += 1;
		q->head= q->head%q->size;
		heap_sort(&stats, &q->items[0], q->head);
		return true;
	}
	else {
		return false;
	}
}


bool dequeue(PriorityQueue *q, Node **n){
	if (q){
	    if (pq_empty(q)){
	    	return false;
	}
	*n = q->items[q->tail];
	q->tail += 1;
	return true;
	} else {
		return false;
	}
}


void pq_print(PriorityQueue *q){
	if (q) {
		for (int i=q->tail; i<q->head; i++){
		printf("pq symbol: %c, pq frequency: %"PRIu64"\n", q->items[i]->symbol, q->items[i]->frequency);
		}
	}
}

