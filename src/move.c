#include <stdio.h>

#include "move.h"
#include "attacks.h"
#include "const.h"
#include "util.h"

int is_square_attacked(int square, int side) {
  // attacked by whie pawn
  if ((side == white) && (pawn_attacks[black][square] & (bitboards[P] & occupancies[white]))) return 1;
  
  // attacked by whie pawn
  if ((side == black) && (pawn_attacks[white][square] & (bitboards[P] & occupancies[black]))) return 1;

  // attacked by knight
  if (knight_attack[square] & ((side == white) ? (bitboards[N] & occupancies[white]) : (bitboards[N] & occupancies[black]))) return 1;

  // attacked by king
  if (king_attack[square] & ((side == white) ? (bitboards[K] & occupancies[white]) : (bitboards[K] & occupancies[black]))) return 1;

  // inti white and black occupied squares
  U64 occupied = occupancies[white] | occupancies[black];

  // attacked by bishop
  if (get_bishop_attacks(square, occupied) & ((side == white) ? (bitboards[B] & occupancies[white]) : (bitboards[B] & occupancies[black]))) return 1;

  // attacked by rook 
  if (get_rook_attacks(square,occupied) & ((side == white) ? (bitboards[R] & occupancies[white]) : (bitboards[R] & occupancies[black]))) return 1;

  // attacked by king 
  if (get_queen_attacks(square, occupied) & ((side == white) ? (bitboards[Q] & occupancies[white]) : (bitboards[Q] & occupancies[black]))) return 1;

  return 0;
}

void print_attacked_squares(int side) {
  printf("\n");
  // loop over board ranks
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      // init square
      int square = rank * 8 + file;

      // print ranks
      if (!file) {
        printf("%d ", 8 - rank);
      }

      // check whether current square is being attacked
      printf(" %d" , is_square_attacked(square, side) ? 1 : 0);
    }
    printf("\n");
  }
  // print board files
  printf("\n   a b c d e f g h\n\n");
}

void generate_moves() {
  // define source & target squares
  int source_square, target_square;

  // define current piece's bitboard copy & it's attacks
  U64 bitboard, attacks;

  // loop over all bitboards
  for (int piece = P; piece <= k; ++piece) {
    // init piece bitboard copy
    if (piece >= P && piece <= K) {
      // get white piece
      bitboard = bitboards[piece] & occupancies[white];
    } else {
      // get black piece
      bitboard = bitboards[piece - 6] & occupancies[black];
    }

    // generate white pawns & white king castleing moves
    if (side == white) {
      // pick up white pawn bitboards index
      if (piece == P) {
        // loop over white pawns within white pawn bitboard
        while (bitboard) {
          // init source square
          source_square = get_ls1b_index(bitboard);
          // printf("white pawn: %s\n", square_to_coordinates[source_square]);

          // init target square
          target_square = source_square - 8;

          // generate quiet pawn moves
          if (!(target_square < a8) && !get_bit(occupancies[white] | occupancies[black], target_square)) {
            // pawn promotion
            if (source_square >= a7 && source_square <= h7) {
              // add move into a move list (placeholder)
              printf("pawn promotion: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promotion: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promotion: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promotion: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);

            } else {
              // one square ahead pawn move
              printf("pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              
              // two squares ahead pawn move
              if ((source_square >= a2 && source_square <= h2) && !get_bit(occupancies[white] | occupancies[black], target_square - 8)) {
                printf("double pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square - 8]);
              }
            }
          }
          
          // init pawn attacks bitboard
          attacks = pawn_attacks[side][source_square] & occupancies[black];

          // generate pawn captures
          while (attacks) {
            // init target square
            target_square = get_ls1b_index(attacks);
            
            // pawn promotion
            if (source_square >= a7 && source_square <= h7) {
              // add move into a move list (placeholder)
              printf("pawn promotion capture: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promotion capture: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promotion capture: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promotion capture: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);

            } else {
              // one square ahead pawn move
              printf("pawn capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
            }
            // generate en passant captures
            if (enpassent != no_sq) {
              // look up pawn attacks and bitwise AND with en passent square (bit) 
              U64 enpassent_attacks = pawn_attacks[side][source_square] & (1ULL << enpassent);

              // make sure en passent capture available
              if (enpassent_attacks) {
                // init enpassant capture target square
                int target_enpassant = get_ls1b_index(enpassent_attacks);
                printf("pawn enpassant capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_enpassant]);
              }
            }

            // pop ls1b of the pawn attack
            pop_bit(&attacks, target_square);
          }
          
          // pop ls1b from piece bitboard copy
          pop_bit(&bitboard, source_square);
        }
      }
    }
    // generate black pawns & white king castleing moves
    else {
      // pick up black pawn bitboards index
      if (piece == p) {
        // loop over black pawns within white pawn bitboard
        while (bitboard) {
          // init source square
          source_square = get_ls1b_index(bitboard);
          // printf("white pawn: %s\n", square_to_coordinates[source_square]);

          // init target square
          target_square = source_square + 8;

          // generate quiet pawn moves
          if (!(target_square > h1) && !get_bit(occupancies[white] | occupancies[black], target_square)) {
            // pawn promotion
            if (source_square >= a2 && source_square <= h2) {
              // add move into a move list (placeholder)
              printf("pawn promotion: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promotion: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promotion: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promotion: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);

            } else {
              // one square ahead pawn move
              printf("pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              
              // two squares ahead pawn move
              if ((source_square >= a7 && source_square <= h7) && !get_bit(occupancies[white] | occupancies[black], target_square + 8)) {
                printf("double pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square + 8]);
              }
            }
          }

          // init pawn attacks bitboard
          attacks = pawn_attacks[side][source_square] & occupancies[white];

          // generate pawn captures
          while (attacks) {
            // init target square
            target_square = get_ls1b_index(attacks);
            
            // pawn promotion
            if (source_square >= a2 && source_square <= h2) {
              // add move into a move list (placeholder)
              printf("pawn promotion capture: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promotion capture: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promotion capture: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
              printf("pawn promotion capture: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);

            } else {
              // one square ahead pawn move
              printf("pawn capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
            }
            // generate en passant captures
            if (enpassent != no_sq) {
              // look up pawn attacks and bitwise AND with en passent square (bit) 
              U64 enpassent_attacks = pawn_attacks[side][source_square] & (1ULL << enpassent);

              // make sure en passent capture available
              if (enpassent_attacks) {
                // init enpassant capture target square
                int target_enpassant = get_ls1b_index(enpassent_attacks);
                printf("pawn enpassant capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_enpassant]);
              }
            }

            // pop ls1b of the pawn attack
            pop_bit(&attacks, target_square);
          }
          
          // pop ls1b from piece bitboard copy
          pop_bit(&bitboard, source_square);
        }
      }
    }

    // generate kngiht moves

    // generate bishop moves

    // generate rook moves

    // generate queen moves

    // generate king moves

  }
  
}