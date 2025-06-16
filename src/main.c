#include <stdio.h>
#include "const.h"
#include "move.h"
#include "util.h"

#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "

int main() {
  init_all();
  
  moves move_list[1];
  move_list->count = 0;

  add_move(move_list, encode_move(e7, e8, P, Q, 0, 0, 0, 0));

  print_move_list(move_list);

  return 0;
}