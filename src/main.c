#include <stdio.h>
#include "const.h"
#include "util.h"
#include "attacks.h"

int main() {
  init_all();

  U64 occupancy = 0x8142214080000ULL;
  U64 bitboard = get_queen_attacks(d4, occupancy);
  print_bitboard(bitboard);

  return 0;
}