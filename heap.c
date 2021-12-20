//
//

#include "node.h"
#include "stats.h"
#include <stdio.h>
#include <stdbool.h>

uint32_t min_child(Stats *stats, Node **A, uint32_t first, uint32_t last){
	uint32_t left = 2*first;
	uint32_t right = left+1;
	if ((right<=last) && ((1)==cmp(stats, A[right-1]->frequency, A[left-1]->frequency))){
		return right;
	}
	return left;
}


void fix_heap(Stats *stats, Node **A, uint32_t first, uint32_t last ){
	bool found = false;
	uint32_t mother = first;
	uint32_t great = min_child(stats, A, mother, last);

	while ((mother <= (last/2)) && (!found)){
		if ((-1)==cmp(stats, A[mother -1]->frequency, A[great-1]->frequency)){
			swap(stats, &A[mother-1], &A[great-1]);

			mother = great;
			great = min_child(stats, A, mother, last);
		}
		else {
			found = true;
		}
	}
}



void build_heap(Stats *stats, Node **A, uint32_t first, uint32_t last){
	for (uint32_t father = last/2; father>(first-1); father--){
		fix_heap(stats, A, father, last);
	}
}


//  heap_sort that allows recursion and a lower array index
//
void heap_sort(Stats *stats, Node **A, uint32_t n) {
	uint32_t first = 1;
	uint32_t last = n;
	build_heap(stats, A, first, last);
	for (uint32_t leaf = last; leaf>first; leaf--){
		swap(stats, &A[first-1], &A[leaf-1]);
		fix_heap(stats, A, first, leaf-1);
	}
}

