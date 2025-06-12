#include <stdio.h>
#include "const.h"
#include "util.h"
// #include "attacks.h"

int main() {
  init_all();

  occupancies[white] = 0xffff000000000000ULL;
  occupancies[black] = 0xffffULL;

  // set board starting position 
  bitboards[P] = 0xff00000000ff00ULL;
  bitboards[N] = 0x4200000000000042ULL;
  bitboards[B] = 0x2400000000000024ULL;
  bitboards[R] = 0x8100000000000081ULL;
  bitboards[Q] = 0x800000000000008ULL;
  bitboards[K] = 0x1000000000000010ULL;

  // init side
  side = black;
  
  // init en passant
  enpassent = e4;

  // init castling
  castle |= wk;
  castle |= wq;
  castle |= bk;
  castle |= bq;

  print_board();
  
  // print all piece bitboard
  for (int piece = P; piece <= K; ++piece) {
    print_bitboard(bitboards[piece]);
  }
  
  return 0;
}