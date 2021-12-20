#pragma once

#include <stdint.h>
#include "node.h"

typedef struct {
    uint64_t moves;
    uint64_t compares;
} Stats;

int cmp(Stats *stats, uint32_t x, uint32_t y);

uint32_t move(Stats *stats, uint32_t x);

void swap(Stats *stats, Node **x, Node **y);

void reset(Stats *stats);
