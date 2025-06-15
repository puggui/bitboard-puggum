#include <stdio.h>
// #include "const.h"
#include "move.h"
#include "util.h"
// #include "attacks.h"

#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define fen "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1 "

int main() {
  init_all();

  parse_fen(fen);
  print_board();
  
  // generate moves
  generate_moves();

  return 0;
}