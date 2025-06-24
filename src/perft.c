#include <string.h>

#include "perft.h"
#include "move.h"
#include "util.h"
#include "const.h"

long nodes;
long captures;
long ep;
long castles;
long promotions;
long checks;

void perft_driver(int depth) {
  // recursion escape condition
  if (depth == 0) {
    // increment node count (count reached positions)
    nodes++;
    return;
  }
  // create move list instance
  moves move_list[1];

  // generate moves
  generate_moves(move_list);

  // loop over generated moves
  for (int move_count = 0; move_count < move_list->count; ++move_count) {
    // preserve board state
    copy_board();

    // make move
    if (!make_move(move_list->moves[move_count], all_moves)) {
      continue; // skip to the next move 
    } 

    // call perft driver recursively
    perft_driver(depth - 1);

    // restore board state
    take_back();
  }
}