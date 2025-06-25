#include <stdio.h>
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

void perft_test(int depth) {
  printf("perft test\n");

  // create move list instance
  moves move_list[1];

  // generate moves
  generate_moves(move_list);
  
  int start_time = get_time_ms();

  // loop over generated moves
  for (int move_count = 0; move_count < move_list->count; ++move_count) {
    // preserve board state
    copy_board();

    // init move
    int move = move_list->moves[move_count];

    // make move
    if(!make_move(move, all_moves)) continue;

    // cumulative nodes
    long cumulative_nodes = nodes;

    // call perft driver recursively
    perft_driver(depth - 1);

    // cumulative nodes
    long old_nodes = nodes - cumulative_nodes;

    // restore board state
    take_back();
    
    // print move
    printf("%s%s%c - %ld\n", square_to_coordinates[get_move_source(move)], square_to_coordinates[get_move_target(move)], promoted_pieces[get_move_promoted(move)], old_nodes);
  }

  printf("\ndepth: %d\n", depth);
  printf("nodes: %ld\n", nodes);
  printf("time: %d\n", get_time_ms() - start_time);
}