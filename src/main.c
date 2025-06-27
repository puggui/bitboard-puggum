#include <stdio.h>
#include <string.h>
#include "const.h"
#include "move.h"
#include "perft.h"
#include "util.h"
#include "uci.h"

#define white_enpassant_test "r3k2r/p11pqpb1/bn2pnp1/2pPN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq c6 0 1 "
#define black_enpassant_test "r3k2r/p11pqpb1/bn2pnp1/2pPN3/Pp2P3/2N2Q1p/1PPBBPPP/R3K2R b KQkq a3 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define king_in_check_test "r3k2r/p1ppqpb1/1n2pnp1/3PN3/1p2P3/2N2Q1p/PPPBqPPP/R3K2R w KQkq - 0 1 "
#define empty_board "8/8/8/8/8/8/8/8 b - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

int main() {
  init_all();

  parse_position("position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 moves a2a4");
  // parse_position("position startpos moves e2e4 e7e5 g1f3");
  print_board();
  
  return 0;
}