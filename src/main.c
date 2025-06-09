#include <stdio.h>
#include "const.h"
#include "util.h"
#include "attacks.h"

int main() {
  // init leaper pieces attacks
  init_leapers_attacks(); 

  printf("%u\n", get_random_number());
  printf("%u\n", get_random_number());
  printf("%u\n", get_random_number());
  printf("%u\n", get_random_number());

  return 0;
}