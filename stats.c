//  
//  stats.c creates a structure to store statisics on the sort routines.  The metrics stored are
//  the number of moves to do the sort and the number of comparisons to do the sort.  It has
//  4 functions:  cmp to compare two elements of an array and return -1, 0, or 1 depending
//                move to increment the move cntr 
//                swap increments the move cntr by three and then swaps the two elements using a
//                     temporary variable to store one element of the swap
//                reset zeros the moves and comparison cntr in the stats structure
//
//

#include "stats.h"
#include "node.h"

int cmp(Stats *stats, uint32_t x, uint32_t y) {
	stats->compares += 1;
	if (x<=y) return -1;
	if (x==y) return 0;
	return 1;
}

uint32_t move(Stats *stats, uint32_t x) {
	stats->moves += 1;
	return x;
}

void swap(Stats *stats, Node **x, Node **y) {
	Node *tmp;

	stats->moves += 3;
	tmp = *x;
	*x = *y;
	*y = tmp;
	return;
}

void reset (Stats *stats) {
	stats->moves=0;
	stats->compares=0;
	return;
}

