#pragma once

#include "stats.h"
#include "node.h"

#include <stdint.h>

void heap_sort(Stats *stats, Node **A, uint32_t n);
