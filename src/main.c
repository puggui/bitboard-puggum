#include <stdio.h>
#include "const.h"
#include "move.h"
#include "util.h"
// #include "attacks.h"

int main() {
  init_all();

  // parse_fen("8/8/4n3/3Q4/8/8/8/8 w - -");
  parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  print_board();

  // print all attacked squares on the chess board
  // print_attacked_squares(white);
  print_attacked_squares(black);

  return 0;
}