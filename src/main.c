#include <stdio.h>
#include <string.h>
#include "const.h"
#include "move.h"
#include "perft.h"
#include "util.h"

#define white_enpassant_test "r3k2r/p11pqpb1/bn2pnp1/2pPN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq c6 0 1 "
#define black_enpassant_test "r3k2r/p11pqpb1/bn2pnp1/2pPN3/Pp2P3/2N2Q1p/1PPBBPPP/R3K2R b KQkq a3 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define king_in_check_test "r3k2r/p1ppqpb1/1n2pnp1/3PN3/1p2P3/2N2Q1p/PPPBqPPP/R3K2R w KQkq - 0 1 "
#define empty_board "8/8/8/8/8/8/8/8 b - - "

int main() {
  init_all();

  parse_fen(tricky_position);
  // print_board();
  
  // start tracking time
  int start_time = get_time_ms();

  perft_driver(3);

  // time taken to execute program
  printf("time taken to execute: %d ms\n", get_time_ms() - start_time);
  printf("nodes: %ld\n", nodes);

  return 0;
}