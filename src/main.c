#include <stdio.h>
#include "const.h"
#include "util.h"
// #include "attacks.h"

int main() {
  init_all();
  
  // set white pawn to e2
  set_bit(&bitboards[P], e2);
  print_bitboard(bitboards[P]);

  printf("piece: %c\n", ascii_pieces[P]);
  printf("piece: %c\n", ascii_pieces[char_pieces['K']]);
  printf("%d", char_pieces['K']);

  return 0;
}