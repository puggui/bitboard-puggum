#include <stdio.h>
#include "const.h"
#include "util.h"
#include "attacks.h"

int main() {
  init_all();

  // define test bitboard
  U64 occupancy = 0X1220040000A4080ULL;

  print_bitboard(occupancy);

  // print bishop attacks
  // print_bitboard(get_bishop_attacks(d4, occupancy));
  print_bitboard(get_rook_attacks(d4, occupancy));

  return 0;
}