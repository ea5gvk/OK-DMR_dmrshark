/*
 * This file is part of dmrshark.
 *
 * dmrshark is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dmrshark is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dmrshark.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef BASE_H_
#define BASE_H_

#include "types.h"

uint8_t base_hexdatatodata(char *hexdata);
uint8_t base_bitstobyte(flag_t bits[8]);
void base_bitstobytes(flag_t *bits, uint16_t bits_length, uint8_t *bytes, uint16_t bytes_length);

void base_process(void);

void base_init(void);
void base_deinit(void);

#endif
