#include <stdio.h>
// #include "const.h"
#include "const.h"
#include "move.h"
#include "util.h"
// #include "attacks.h"

#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "

/*
        binary move bits                               hexidecimal constants
  
  0000 0000 0000 0000 0011 1111    source square       0x3f
  0000 0000 0000 1111 1100 0000    target square       0xfc0
  0000 0000 1111 0000 0000 0000    piece               0xf000
  0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
  0001 0000 0000 0000 0000 0000    capture flag        0x100000
  0010 0000 0000 0000 0000 0000    double push flag    0x200000
  0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
  1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/

void print_binary_move_bits(int num) {
    for (int i = 23; i >= 0; --i) {
      printf("%d", (num >> i) & 1);
      if (i % 4 == 0) printf(" ");
    }
    printf("\n");
}

int main() {
  init_all();

  parse_fen(tricky_position);
  print_board();

  // create move
  int move = 0;

  // encode target square of h1 (63)
  move = (move | 63) << 6;

  // print move in binary
  print_binary_move_bits(move);
  
  // extract target square from the move
  int target_square = (move & 0xfc0) >> 6;

  printf("target square: %d  %s\n", target_square, square_to_coordinates[target_square]);

  return 0;
}