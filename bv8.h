#pragma once

// BSD 2-Clause License
//
// Copyright (c) 2021, Darrell Long
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define BITS_PER_UNIT 8

typedef struct bitV {
  uint8_t *v;
  uint32_t l;
} bitV;

static inline bitV *newVec(uint32_t l) {
  bitV *v = (bitV *) malloc(sizeof(bitV));
  if (v) {
    uint32_t bytes = l / BITS_PER_UNIT + (l % BITS_PER_UNIT ? 1 : 0);
    v->v = (uint8_t *) calloc(bytes, sizeof(uint8_t));
    v->l = l;
    return v;
  } else {
    return (bitV *) 0;
  }
}

static inline void delVec(bitV *v) {
  if (v && v->v) {
    free(v->v);
  }
  if (v) {
    free(v);
  }
  return;
}

static inline void setBit(bitV *x, uint32_t k) {
  if (x && x->v) {
    x->v[k / BITS_PER_UNIT] |= (0x1 << k % BITS_PER_UNIT);
  }
  return;
}

static inline void clrBit(bitV *x, uint32_t k) {
  if (x && x->v) {
    x->v[k / BITS_PER_UNIT] &= ~(0x1 << k % BITS_PER_UNIT);
  }
  return;
}

static inline bool getBit(bitV *x, uint32_t k) { // We must assume a valid pointer
  return (x->v[k / BITS_PER_UNIT] >> k % BITS_PER_UNIT) & 0x1;
}

static inline uint32_t lenVec(bitV *x) {
  return x ? x->l : 0;
}
