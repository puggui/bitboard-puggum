#include <stdio.h>

#include "search.h"
#include "const.h"
#include "util.h"

// search position placeholder function
void search_position(int depth) {
  // bestmove placeholder
  printf("bestmove d2d4\n");
}

int evaluate() {
  // static evaluation score
  int score = 0;

  // current pieces bitboard copy
  U64 bitboard;

  // init piece and square
  int piece, square;

  // loop over piece bitboard
  for (int bb_piece = P; bb_piece <= k; ++bb_piece) {
    // init piece bitboard copy
    if (bb_piece <= K) {
      bitboard = bitboards[bb_piece] & occupancies[white];
    } else {
      bitboard = bitboards[bb_piece - p] & occupancies[black];
    }
    
    while (bitboard) {
      // init piece
      piece = bb_piece;

      // init square
      square = get_ls1b_index(bitboard);

      // score material weight 
      score += material_score[piece];

      // pop ls1b
      pop_bit(&bitboard, square);
    }
  }
  // return final evaluation based on side
  return side == white ? score : -score;
}