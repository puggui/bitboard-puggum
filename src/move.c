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
  if (knight_attacks[square] & ((side == white) ? (bitboards[N] & occupancies[white]) : (bitboards[N] & occupancies[black]))) return 1;

  // attacked by king
  if (king_attacks[square] & ((side == white) ? (bitboards[K] & occupancies[white]) : (bitboards[K] & occupancies[black]))) return 1;

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

    // generate white pawns & white king castling moves
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
      // castling moves
      if (piece == K) {
        // king side castling is available
        if (castle & wk) {
          // make sure squares between king and king side rook are empty
          if (!get_bit(occupancies[white] | occupancies[black], f1) && 
              !get_bit(occupancies[white] | occupancies[black], g1)) {
            // make sure king and the f1 squares are not under attack
            // don't need to check king's target square (g1) since we will have to check if the king moves into check at the end anyways
            if (!is_square_attacked(e1, black) && 
                !is_square_attacked(f1, black)) {
              printf("castling move: e1g1\n");
            }
          }
        }

        // queen side castling is available
        if (castle & wq) {
          // make sure squares between king and queen side rook are empty
          if (!get_bit(occupancies[white] | occupancies[black], b1) && 
              !get_bit(occupancies[white] | occupancies[black], c1) && 
              !get_bit(occupancies[white] | occupancies[black], d1)) {
            // make sure king and the c1, d1 squares are not under attack
            // don't need to check king's target square (c1) since we will have to check if the king moves into check at the end anyways
            if (!is_square_attacked(d1, black) &&
                !is_square_attacked(e1, black)) {
              printf("castling move: e1c1\n");
            }
          }
        }
      }
    }
    // generate black pawns & white king castling moves
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

      // castling moves
      if (piece == k) {
        // king side castling is available
        if (castle & bk) {
          // make sure squares between king and king side rook are empty
          if (!get_bit(occupancies[white] | occupancies[black], f8) && 
              !get_bit(occupancies[white] | occupancies[black], g8)) {
            // make sure king and the f8 squares are not under attack
            // don't need to check king's target square (g8) since we will have to check if the king moves into check at the end anyways
            if (!is_square_attacked(e8, white) && 
                !is_square_attacked(f8, white)) {
              printf("castling move: e8g8\n");
            }
          }
        }

        // queen side castling is available
        if (castle & bq) {
          // make sure squares between king and queen side rook are empty
          if (!get_bit(occupancies[white] | occupancies[black], b8) && 
              !get_bit(occupancies[white] | occupancies[black], c8) && 
              !get_bit(occupancies[white] | occupancies[black], d8)) {
            // make sure king and the c8, d8 squares are not under attack
            // don't need to check king's target square (c8) since we will have to check if the king moves into check at the end anyways
            if (!is_square_attacked(d8, white) &&
                !is_square_attacked(e8, white)) {
              printf("castling move: e8c8\n");
            }
          }
        }
      }
    }

    // generate knight moves
    if ((side == white) ? piece == N : piece == n) {
      // loop over source squares of piece bitboard copy
      while (bitboard) {
        // init source square
        source_square = get_ls1b_index(bitboard);

        // init piece attacks in order to get set of target squares
        attacks = knight_attacks[source_square] & ((side == white) ? ~occupancies[white] : ~occupancies[black]);
        
        // loop over target squares available from generated attacks
        while (attacks) {
          // init target square
          target_square = get_ls1b_index(attacks);

          // quiet move (if target square is free)
          if (!get_bit((side == white) ? occupancies[black] : occupancies[white], target_square)){
            printf("knight move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          } else {
            // capture move
            printf("knight capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          // pop ls1b in current attack set
          pop_bit(&attacks, target_square);
        }

        // pop ls1b of the current piece bitboard copy
        pop_bit(&bitboard, source_square);
      }
    }

    // generate bishop moves
    if ((side == white) ? piece == B : piece == b) {
      // loop over source squares of piece bitboard copy
      while (bitboard) {
        // init source square
        source_square = get_ls1b_index(bitboard);

        // init piece attacks in order to get set of target squares
        attacks = get_bishop_attacks(source_square, occupancies[white] | occupancies[black]) & ((side == white) ? ~occupancies[white] : ~occupancies[black]);
        
        // loop over target squares available from generated attacks
        while (attacks) {
          // init target square
          target_square = get_ls1b_index(attacks);

          // quiet move (if target square is free)
          if (!get_bit((side == white) ? occupancies[black] : occupancies[white], target_square)){
            printf("bishop move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          } else {
            // capture move
            printf("bishop capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          // pop ls1b in current attack set
          pop_bit(&attacks, target_square);
        }

        // pop ls1b of the current piece bitboard copy
        pop_bit(&bitboard, source_square);
      }
    }
    // generate rook moves
    if ((side == white) ? piece == R : piece == r) {
      // loop over source squares of piece bitboard copy
      while (bitboard) {
        // init source square
        source_square = get_ls1b_index(bitboard);

        // init piece attacks in order to get set of target squares
        attacks = get_rook_attacks(source_square, occupancies[white] | occupancies[black]) & ((side == white) ? ~occupancies[white] : ~occupancies[black]);
        
        // loop over target squares available from generated attacks
        while (attacks) {
          // init target square
          target_square = get_ls1b_index(attacks);

          // quiet move (if target square is free)
          if (!get_bit((side == white) ? occupancies[black] : occupancies[white], target_square)){
            printf("rook move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          } else {
            // capture move
            printf("rook capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          // pop ls1b in current attack set
          pop_bit(&attacks, target_square);
        }

        // pop ls1b of the current piece bitboard copy
        pop_bit(&bitboard, source_square);
      }
    }

    // generate queen moves
    if ((side == white) ? piece == Q : piece == q) {
      // loop over source squares of piece bitboard copy
      while (bitboard) {
        // init source square
        source_square = get_ls1b_index(bitboard);

        // init piece attacks in order to get set of target squares
        attacks = get_queen_attacks(source_square, occupancies[white] | occupancies[black]) & ((side == white) ? ~occupancies[white] : ~occupancies[black]);
        
        // loop over target squares available from generated attacks
        while (attacks) {
          // init target square
          target_square = get_ls1b_index(attacks);

          // quiet move (if target square is free)
          if (!get_bit((side == white) ? occupancies[black] : occupancies[white], target_square)){
            printf("queen move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          } else {
            // capture move
            printf("queen capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          // pop ls1b in current attack set
          pop_bit(&attacks, target_square);
        }

        // pop ls1b of the current piece bitboard copy
        pop_bit(&bitboard, source_square);
      }
    }

    // generate king moves
    if ((side == white) ? piece == K : piece == k) {
      // loop over source squares of piece bitboard copy
      while (bitboard) {
        // init source square
        source_square = get_ls1b_index(bitboard);

        // init piece attacks in order to get set of target squares
        attacks = king_attacks[source_square] & ((side == white) ? ~occupancies[white] : ~occupancies[black]);
        
        // loop over target squares available from generated attacks
        while (attacks) {
          // init target square
          target_square = get_ls1b_index(attacks);

          // quiet move (if target square is free)
          if (!get_bit((side == white) ? occupancies[black] : occupancies[white], target_square)){
            printf("rook move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          } else {
            // capture move
            printf("rook capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
          }

          // pop ls1b in current attack set
          pop_bit(&attacks, target_square);
        }

        // pop ls1b of the current piece bitboard copy
        pop_bit(&bitboard, source_square);
      }
    }

  }
  
}

/*
        binary move bits                               hexidecimal constants
  
  0000 0000 0000 0000 0011 1111    source square       0x3f
  0000 0000 0000 1111 1100 0000    target square       0xfc0
  0000 0000 1111 0000 0000 0000    piece               0xf000
  0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
  0001 0000 0000 0000 0000 0000    capture flag        0x100000
  0010 0000 0000 0000 0000 0000    double push flag    0x200000
  0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
  1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/

int encode_move(int source, int target, int piece, int promoted, int capture, int doublepush, int enpassant, int castling) {
  return (source) | ( target << 6) | (piece << 12) | (promoted << 16) | (capture << 20) | (doublepush << 21) | (enpassant << 22) | (castling << 23);
}

int get_move_source(int move) {
  return (move & 0x3f);
}

int get_move_target(int move) {
  return (move & 0xfc0) >> 6;
}

int get_move_piece(int move) {
  return (move & 0xf000) >> 12;
}

int get_move_promoted(int move) {
  return (move & 0xf0000) >> 16;
}

// we dont >> 20 for performance
int get_move_capture(int move) {
  return (move & 0x100000);
}

int get_move_double(int move) {
  return (move & 0x200000);
}

// extract enpassant flag
int get_move_enpassant(int move) {
  return (move & 0x400000);
}

// extract castling flag
int get_move_castling(int move) {
  return (move & 0x800000);
}