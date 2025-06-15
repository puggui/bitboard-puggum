#include <stdio.h>
#include "const.h"
#include "move.h"
#include "util.h"

#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "

void print_binary_move_bits(int num) {
    for (int i = 23; i >= 0; --i) {
      printf("%d", (num >> i) & 1);
      if (i % 4 == 0) printf(" ");
    }
    printf("\n");
}

int main() {
  init_all();

  int move = encode_move(e7, e8, P, Q, 0, 0, 0, 0);

  // extract move items
  int source_square = get_move_source(move);
  int target_square = get_move_target(move);
  int piece = get_move_piece(move);
  int promoted = get_move_promoted(move);

  printf("source square: %s\n", square_to_coordinates[source_square]);
  printf("target square: %s\n", square_to_coordinates[target_square]);
  printf("piece: %c\n", ascii_pieces[piece]);
  printf("promoted: %c\n", ascii_pieces[promoted]);
  printf("capture flag: %d\n", get_move_capture(move));
  printf("double pawn push flag: %d\n", get_move_double(move));
  printf("enpassent flag: %d\n", get_move_enpassant(move));
  printf("castling flag: %d\n", get_move_castling(move));

  return 0;
}