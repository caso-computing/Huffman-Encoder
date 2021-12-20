#pragma once

#include "code.h"
#include "io.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void flush_outbuffer(int outfile);
bool write_byte(int outfile, uint8_t x);
