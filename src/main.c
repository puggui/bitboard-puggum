#include <stdio.h>
#include "util.h"
#include "attacks.h"

int main() {
  // init leaper pieces attacks
  init_leapers_attacks(); 

  print_bitboard(get_random_u64_number());
  print_bitboard(get_random_u64_number() & get_random_u64_number() & get_random_u64_number()); 

  return 0;
}