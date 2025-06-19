#include <stdio.h>
#include <string.h>
#include "const.h"
// #include "move.h"
#include "util.h"

#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define empty_board "8/8/8/8/8/8/8/8 b - - "

int main() {
  init_all();

  parse_fen(tricky_position);
  print_board();
  
  // preserve board state
  copy_board();

  // parse fen
  parse_fen(empty_board);
  print_board();

  // restore board state
  take_back();

  print_board();

  return 0;
}