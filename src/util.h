#ifndef UTIL_H
#define UTIL_H

#include "const.h"

int count_bits(U64 bitboard);

// get least siginifcant 1st bit index
int get_ls1b_index(U64 bitboard);

void print_bitboard(U64 bitboard);

U64 get_bit(U64 bitboard, int square);
void set_bit(U64* bitboard, int square);
void pop_bit(U64* bitboard, int square);

#endif