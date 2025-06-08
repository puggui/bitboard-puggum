#include <stdio.h>
#include "const.h"
#include "util.h"
#include "attacks.h"

int main() {
  // init occupancy bitboard
  U64 block = 0ULL;
  set_bit(&block, d7);
  set_bit(&block, d1);
  set_bit(&block, b4);
  set_bit(&block, g4);
  print_bitboard(block);

  printf("bit count: %d\n", count_bits(block));
  // init leaper pieces attacks
  init_leapers_attacks(); 

  U64 bitboard = mask_king_attacks(g4);
  print_bitboard(bitboard);

  return 0;
}