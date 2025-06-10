#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include "const.h"

int count_bits(U64 bitboard);

// get least siginifcant 1st bit index
int get_ls1b_index(U64 bitboard);

void print_bitboard(U64 bitboard);

U64 get_bit(U64 bitboard, int square);
void set_bit(U64* bitboard, int square);
void pop_bit(U64* bitboard, int square);

// psuedo random number
extern uint32_t random_state;

// pseudo random number state
uint32_t get_random_u32_number();

// generate 64 bit psuedo legal numbers
U64 get_random_u64_number();

// generate magic number candidate
U64 generate_magic_number();

// find suitable magic number
U64 find_magic_number(int square, int relevant_bits, int bishop);

// init magic numbers
void init_magic_number();

void init_all();

#endif