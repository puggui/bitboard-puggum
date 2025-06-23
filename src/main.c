#include <stdio.h>
#include <string.h>
#include "const.h"
#include "move.h"
#include "util.h"

#define white_enpassant_test "r3k2r/p11pqpb1/bn2pnp1/2pPN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq c6 0 1 "
#define black_enpassant_test "r3k2r/p11pqpb1/bn2pnp1/2pPN3/Pp2P3/2N2Q1p/1PPBBPPP/R3K2R b KQkq a3 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define empty_board "8/8/8/8/8/8/8/8 b - - "

int main() {
  init_all();

  parse_fen(tricky_position);
  print_board();
  
  // create move list instance
  moves move_list[1];

  // generate moves
  generate_moves(move_list);

  // loop over generated moves
  for (int move_count = 0; move_count < move_list->count; ++move_count) {
    // init move
    int move = move_list->moves[move_count];

    // preserve board state
    copy_board();

    // make move
    if (!make_move(move, all_moves)) {
      continue; 
    } 
    printf("make move\n");
    print_board();
    getchar();

    // take back
    take_back();
    printf("take back\n");
    print_board();
    getchar();
  }

  return 0;
}