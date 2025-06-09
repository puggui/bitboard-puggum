#include <stdio.h>
#include "const.h"
#include "util.h"
#include "attacks.h"

int main() {
  // init leaper pieces attacks
  init_leapers_attacks(); 

  // mask peice attacks at given square
  U64 attack_mask = mask_rook_attack(a1);
  // U64 attack_mask = mask_bishop_attacks(d4);

  // loop over occupancy indicies 
  // 4096 is from 2^12 number of all possible variations of occupancy mask for rook at a1
  for (int index = 0; index < 4096; ++index) {
    // init occupancy
    print_bitboard(set_occupancy(index, count_bits(attack_mask), attack_mask));
    getchar();
  }

  return 0;
}