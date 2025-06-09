#include <stdio.h>
#include "const.h"
#include "util.h"
#include "attacks.h"

int main() {
  // init leaper pieces attacks
  init_leapers_attacks(); 

  // generate rook/bishop occupancy bit count
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      int square = rank * 8 + file;
      // printf("%d, ", count_bits(mask_rook_attack(square)));
      printf("%d, ", count_bits(mask_bishop_attacks(square)));
    }
    printf("\n");
  }

  return 0;
}