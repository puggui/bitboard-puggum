#include <stdio.h>
#include "util.h"
#include "const.h"

// count bits within a bitboard 
int count_bits(U64 bitboard) {
  // bit counter
  int count = 0;
  
  // consecutively reset least significant 1st big
  while (bitboard) {
    // increament count
    count++;

    // reset least significant 1st bit
    bitboard &= bitboard - 1;
  }

  return count;
}

// get least significant 1st bit index
int get_ls1b_index(U64 bitboard) {
  // make sure bitboard is not 0
  if (bitboard) {
    // count trailing bits before LS1B
    return count_bits((bitboard & -bitboard) - 1);
  } else {
    // else return illegal index
    return -1;
  }
}

void print_bitboard(U64 bitboard) {
  printf("\n");
  // loop over board rank
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      int square = rank * 8 + file;

      // print ranks
      if (!file) printf("%d ", 8 - rank);

      // get bit state (1 or 0)
      printf(" %d", get_bit(bitboard, square) ? 1 : 0);
    }
    printf("\n");
  }
  // print board files
  printf("\n   a b c d e f g h\n\n");
  
  // print bitboard as unsigned decimal number
  printf("   Bitboard: 0x%llx\n\n", bitboard);
}

U64 get_bit(U64 bitboard, int square) {
 return bitboard & (1ULL << square);
}

void set_bit(U64* bitboard, int square) {
  *bitboard |= (1ULL << square);
}

void pop_bit(U64* bitboard, int square) {
  get_bit(*bitboard, square) ? *bitboard ^= (1ULL << square) : 0;
}

uint32_t state = 1804289383;

uint32_t get_random_number() {
  // get current state
  unsigned int number = state;

  // XOR shift algorithm
  number ^= number << 13;
  number ^= number >> 17;
  number ^= number << 5;

  // update random number state
  state = number;

  return number;
}