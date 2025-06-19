#include <stdio.h>
// #include "const.h"
#include "move.h"
#include "util.h"

#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define fen "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPpP/R3K2R b KQkq a3 0 1 "

int main() {
  init_all();

  parse_fen(tricky_position);
  print_board();
  
  moves move_list[1];
  generate_moves(move_list);

  print_move_list(move_list);

  return 0;
}