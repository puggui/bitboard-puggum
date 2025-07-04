#include <stdio.h>
#include <string.h>
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

  // attacked by queen 
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

void generate_moves(moves* move_list) {
  // init move count
  move_list->count = 0;
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
              add_move(move_list, encode_move(source_square, target_square, piece, Q, 0, 0, 0, 0));
              add_move(move_list, encode_move(source_square, target_square, piece, R, 0, 0, 0, 0));
              add_move(move_list, encode_move(source_square, target_square, piece, B, 0, 0, 0, 0));
              add_move(move_list, encode_move(source_square, target_square, piece, N, 0, 0, 0, 0));

            } else {
              // one square ahead pawn move
              add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
              
              // two squares ahead pawn move
              if ((source_square >= a2 && source_square <= h2) && !get_bit(occupancies[white] | occupancies[black], target_square - 8)) {
                add_move(move_list, encode_move(source_square, target_square - 8, piece, 0, 0, 1, 0, 0));
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
              add_move(move_list, encode_move(source_square, target_square, piece, Q, 1, 0, 0, 0));
              add_move(move_list, encode_move(source_square, target_square, piece, R, 1, 0, 0, 0));
              add_move(move_list, encode_move(source_square, target_square, piece, B, 1, 0, 0, 0));
              add_move(move_list, encode_move(source_square, target_square, piece, N, 1, 0, 0, 0));

            } else {
              // one square ahead pawn move
              add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
            }

            // pop ls1b of the pawn attack
            pop_bit(&attacks, target_square);
          }

          // generate en passant captures
          if (enpassant != no_sq) {
            // look up pawn attacks and bitwise AND with en passent square (bit) 
            U64 enpassant_attacks = pawn_attacks[side][source_square] & (1ULL << enpassant);

            // make sure en passent capture available
            if (enpassant_attacks) {
              // init enpassant capture target square
              int target_enpassant = get_ls1b_index(enpassant_attacks);
              add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
            }
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
              add_move(move_list, encode_move(e1, g1, piece, 0, 0, 0, 0, 1));
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
              add_move(move_list, encode_move(e1, c1, piece, 0, 0, 0, 0, 1));
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
              add_move(move_list, encode_move(source_square, target_square, piece, q, 0, 0, 0, 0));
              add_move(move_list, encode_move(source_square, target_square, piece, r, 0, 0, 0, 0));
              add_move(move_list, encode_move(source_square, target_square, piece, b, 0, 0, 0, 0));
              add_move(move_list, encode_move(source_square, target_square, piece, n, 0, 0, 0, 0));

            } else {
              // one square ahead pawn move
              add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
              
              // two squares ahead pawn move
              if ((source_square >= a7 && source_square <= h7) && !get_bit(occupancies[white] | occupancies[black], target_square + 8)) {
                add_move(move_list, encode_move(source_square, target_square + 8, piece, 0, 0, 1, 0, 0));
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
              add_move(move_list, encode_move(source_square, target_square, piece, q, 1, 0, 0, 0));
              add_move(move_list, encode_move(source_square, target_square, piece, r, 1, 0, 0, 0));
              add_move(move_list, encode_move(source_square, target_square, piece, b, 1, 0, 0, 0));
              add_move(move_list, encode_move(source_square, target_square, piece, n, 1, 0, 0, 0));

            } else {
              // one square ahead pawn move
              add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
            }

            // pop ls1b of the pawn attack
            pop_bit(&attacks, target_square);
          }
          
          // generate en passant captures
          if (enpassant != no_sq) {
            // look up pawn attacks and bitwise AND with en passent square (bit) 
            U64 enpassant_attacks = pawn_attacks[side][source_square] & (1ULL << enpassant);

            // make sure en passent capture available
            if (enpassant_attacks) {
              // init enpassant capture target square
              int target_enpassant = get_ls1b_index(enpassant_attacks);
              add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
            }
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
              add_move(move_list, encode_move(e8, g8, piece, 0, 0, 0, 0, 1));
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
              add_move(move_list, encode_move(e8, c8, piece, 0, 0, 0, 0, 1));
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
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
          } else {
            // capture move
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
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
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
          } else {
            // capture move
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
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
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
          } else {
            // capture move
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
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
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
          } else {
            // capture move
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
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
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
          } else {
            // capture move
            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
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

void print_move(int move) {
  if (get_move_promoted(move)) {
    printf("%s%s%c\n", square_to_coordinates[get_move_source(move)], square_to_coordinates[get_move_target(move)], promoted_pieces[get_move_promoted(move)]);
  } else {
    printf("%s%s\n", square_to_coordinates[get_move_source(move)], square_to_coordinates[get_move_target(move)]);
  }
}

void print_move_list(moves* move_list) {
  // do nothing on empty  move list
  if (move_list->count == 0) {
    printf("\n     no moves in the move list\n\n");
    return;
  }

  printf("\n     move    piece     capture   double    enpass    castling\n\n");

  // loop over moves within a move list
  for (int move_count = 0; move_count < move_list->count; ++move_count) {
    // init move
    int move = move_list->moves[move_count];

    // print move
    printf(
      "     %s%s%c    %c         %d         %d         %d         %d\n", 
      square_to_coordinates[get_move_source(move)],
      square_to_coordinates[get_move_target(move)],
      get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ',
      ascii_pieces[get_move_piece(move)],
      get_move_capture(move) ? 1 : 0,
      get_move_double(move) ? 1 : 0,
      get_move_enpassant(move) ? 1 : 0,
      get_move_castling(move) ? 1 : 0
    );
  }
  // print total number of moves
  printf("\n     Total number of moves: %d\n\n", move_list->count);
}

void add_move(moves* move_list, int move) {
  // store move
  move_list->moves[move_list->count] = move;

  // increment move count
  move_list->count++;
}

int make_move(int move, int move_flag) {
  // quiet moves
  if (move_flag == all_moves) {
    // preserve board state
    copy_board();
    
    // parse move
    int source_square = get_move_source(move);
    int target_square = get_move_target(move);
    int piece = get_move_piece(move);
    int promoted_piece = get_move_promoted(move);
    int capture = get_move_capture(move);
    int doublepush = get_move_double(move);
    int enpass = get_move_enpassant(move);
    int castling = get_move_castling(move);

    // remove piece from source square 
    (side == white) ? pop_bit(&bitboards[piece], source_square) : pop_bit(&bitboards[piece - 6], source_square);
    pop_bit(&occupancies[side], source_square);
    

    // handling capture moves
    if (capture) {
      // loop over bitboards opposite to the current side to move
      for (int bb_piece = P; bb_piece <= K; ++bb_piece) {
        // if there is a piece on the target square
        if (get_bit(bitboards[bb_piece], target_square) && get_bit(occupancies[!side], target_square)) {
          // remove it from correspondign bitboard
          pop_bit(&bitboards[bb_piece], target_square);
          pop_bit(&occupancies[!side], target_square);
          break;
        }
      }
    }
    
    // check if promote piece to not result in double work
    if (!promoted_piece) {
      // add piece at the end to not conflict with capture move
      add_piece(piece, target_square);
    }

    // handle pawn promotions
    if (promoted_piece) {
      // erase pawn from target square
      (side == white) ? pop_bit(&bitboards[piece], target_square) : pop_bit(&bitboards[piece - 6], target_square);
      pop_bit(&occupancies[side], target_square);
      // set up promoted piece on chess board
      (side == white) ? set_bit(&bitboards[promoted_piece], target_square) : set_bit(&bitboards[promoted_piece - 6], target_square);
      set_bit(&occupancies[side], target_square);
    }

    // handle enpassant capture
    if (enpass) {
      // erase pawn depending on side to move
      (side == white) ? pop_bit(&bitboards[P], target_square + 8) : pop_bit(&bitboards[P], target_square - 8);
      (side == white) ? pop_bit(&occupancies[black], target_square + 8) : pop_bit(&occupancies[white], target_square - 8);
    }

    // reset en passant square
    enpassant = no_sq;

    // handle double pawn push
    if (doublepush) {
      // set enpassant square depending on side to move
      (side == white) ? (enpassant = target_square + 8) : (enpassant = target_square - 8);
    }

    // handle castling moves
    if (castling) {
      switch (target_square) {
        // white castles king side
        case (g1):
          // move H rook
          pop_bit(&bitboards[R], h1);
          pop_bit(&occupancies[white], h1);
          set_bit(&bitboards[R], f1);
          set_bit(&occupancies[white], f1);
          break;
        // white castles queen side
        case (c1):
          // move A rook
          pop_bit(&bitboards[R], a1);
          pop_bit(&occupancies[white], a1);
          set_bit(&bitboards[R], d1);
          set_bit(&occupancies[white], d1);
          break;
        // black casltes king side
        case (g8):
          // move H rook
          pop_bit(&bitboards[R], h8);
          pop_bit(&occupancies[black], h8);
          set_bit(&bitboards[R], f8);
          set_bit(&occupancies[black], f8);
          break;
        // black castles queen side
        case (c8):
          // move A rook
          pop_bit(&bitboards[R], a8);
          pop_bit(&occupancies[black], a8);
          set_bit(&bitboards[R], d8);
          set_bit(&occupancies[black], d8);
          break;
      }
    }

    // update castling rights
    castle &= castling_rights[source_square]; 
    castle &= castling_rights[target_square]; 

    // change side
    side = !side;

    // make sure that king has not been exposed into a check
    int white_king_square = get_ls1b_index(bitboards[K] & occupancies[white]);
    int black_king_square = get_ls1b_index(bitboards[K] & occupancies[black]);
    if (is_square_attacked((side == white) ? black_king_square : white_king_square, side)) {
      // move is illegal, so take it back
      take_back();

      // return illegal move
      return 0;
    } else {
      // return legal move
      return 1;
    }
  }
  // capture moves
  else {
    // make sure move is a capture
    if (get_move_capture(move)) {
      make_move(move, all_moves);
    }
    // otherwise the move is not a capture
    else {
      // dont make the move
      return 0;
    }
  }
  return 1;
}
