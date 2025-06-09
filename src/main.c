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

  // init leaper pieces attacks
  init_leapers_attacks(); 
  
  // test print LS1B
  int index =  get_ls1b_index(block);
  printf("index: %d\n", index);
  printf("coordinate: %s\n", square_to_coordinates[index]);

  return 0;
}