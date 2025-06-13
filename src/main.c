#include <stdio.h>
// #include "const.h"
#include "move.h"
#include "util.h"
// #include "attacks.h"

int main() {
  init_all();

  // parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPpP/R3K2R b KQkq - 0 1 ");
  print_board();
  
  // generate moves
  generate_moves();

  return 0;
}