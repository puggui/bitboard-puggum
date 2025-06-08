#include <stdio.h>

// define bitboard data type
#define U64 unsigned long long

// enum board squares
enum {
  a8, b8, c8, d8, e8, f8, g8, h8,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a1, b1, c1, d1, e1, f1, g1, h1
};

// side to move (colors)
enum { white, black };

// set/get/pop macros
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square));
#define pop_bit(bitboard, square) (get_bit(bitboard, e4) ? bitboard ^= (1ULL << e4) : 0);

// print bitboard 
void print_bitboard(U64 bitboard) {
  printf("\n");
  // loop over board rank
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      int square = rank * 8 + file;

      // print ranks
      if (!file) printf("%d ", 8 - rank);

      // get bit state (1 or 0)
      printf(" %d", get_bit(bitboard, square) ? 1 : 0);
    }
    printf("\n");
  }
  // print board files
  printf("\n   a b c d e f g h\n\n");
  
  // print bitboard as unsigned decimal number
  printf("   Bitboard: 0x%llx\n\n", bitboard);
}

// ********* Attacks **********
// constant for NOT_A_FILE
const U64 NOT_A_FILE = 0xfefefefefefefefeULL;
const U64 NOT_AB_FILE = 0xfcfcfcfcfcfcfcfcULL;
const U64 NOT_H_FILE = 0x7f7f7f7f7f7f7f7fULL;
const U64 NOT_GH_FILE = 0x3f3f3f3f3f3f3f3fULL;
const U64 NOT_1_RANK = 0xffffffffffffffULL;
const U64 NOT_8_RANK = 0xffffffffffffff00ULL;

// pawn attack table [side][square]
U64 pawn_attacks[2][64];

// knight attack table [square]
U64 knight_attack[64];

// king attack table [square]
U64 king_attack[64];

// generate pawn attacks
U64 mask_pawn_attacks(int side, int square) {
  // result attacks bitboard
  U64 attacks = 0ULL;
    
  // piece bitboard
  U64 bitboard = 0ULL;

  // set piece on board
  set_bit(bitboard, square);

  // white pawn
  if (!side) {
    attacks |= (bitboard >> 7ULL) & NOT_A_FILE;
    attacks |= (bitboard >> 9ULL) & NOT_H_FILE;
  } else { // black pawn
    attacks |= (bitboard << 7ULL) & NOT_H_FILE;
    attacks |= (bitboard << 9ULL) & NOT_A_FILE;
  }
  return attacks;
}

// generate knight attacks
U64 mask_knight_attacks(int square) {
  // result attacks bitboard
  U64 attacks = 0ULL;

  // piece bitboard
  U64 bitboard = 0ULL;

  // set piece on bitboard
  set_bit(bitboard, square);

  // generate knight attacks, shift 6, 10, 15, 17
  attacks |= (bitboard >> 6ULL) & NOT_AB_FILE;
  attacks |= (bitboard << 10ULL) & NOT_AB_FILE;
  attacks |= (bitboard >> 15ULL) & NOT_A_FILE;
  attacks |= (bitboard << 17ULL) & NOT_A_FILE;

  attacks |= (bitboard << 6ULL) & NOT_GH_FILE;
  attacks |= (bitboard >> 10ULL) & NOT_GH_FILE;
  attacks |= (bitboard << 15ULL) & NOT_H_FILE;
  attacks |= (bitboard >> 17ULL) & NOT_H_FILE;

  return attacks;
}

// generate king attacks
U64 mask_king_attacks(int square) {
  // result attack bitboard
  U64 attacks = 0ULL;

  // piece bitboard
  U64 bitboard = 0ULL;

  // set piece on bitboard
  set_bit(bitboard, square);

  // generate king attack, shift 1, 7, 8, 9
  attacks |= (bitboard >> 1ULL) & NOT_H_FILE;
  attacks |= (bitboard >> 7ULL) & NOT_A_FILE;
  if (bitboard >> 8ULL) attacks |= (bitboard >> 8ULL);
  attacks |= (bitboard >> 9ULL) & NOT_H_FILE;

  attacks |= (bitboard << 1ULL) & NOT_A_FILE;
  attacks |= (bitboard << 7ULL) & NOT_H_FILE;
  if (bitboard << 8ULL) attacks |= (bitboard << 8ULL);
  attacks |= (bitboard << 9ULL) & NOT_A_FILE;

  return attacks;
}

// mask bishop attacks
U64 mask_bishop_attacks(int square) {
  // return attack bitboard
  U64 attacks = 0ULL;

  // init ranks & files
  int r, f;

  // init target rank & files
  int tr = square / 8;
  int tf = square % 8;

  // mask relavent bishop occupancy bits
  for (r = tr + 1, f = tf + 1; r < 7 && f < 7; ++r, ++f) attacks |= (1ULL << (r * 8 + f));
  for (r = tr - 1, f = tf + 1; r > 0 && f < 7; --r, ++f) attacks |= (1ULL << (r * 8 + f));
  for (r = tr + 1, f = tf - 1; r < 7 && f > 0; ++r, --f) attacks |= (1ULL << (r * 8 + f));
  for (r = tr - 1, f = tf - 1; r > 0 && f > 0; --r, --f) attacks |= (1ULL << (r * 8 + f));

  return attacks;
}

// mask rook attacks
U64 mask_rook_attack(int square) {
  // return attack bitboard
  U64 attacks = 0ULL;

  // init ranks & files
  int r, f;

  // init target rank & files
  int tr = square / 8;
  int tf = square % 8;

  // mask relavent bishop occupancy bits
  for (r = tr + 1; r < 7; ++r) attacks |= (1ULL << (r * 8 + tf));
  for (r = tr - 1; r > 0; --r) attacks |= (1ULL << (r * 8 + tf));
  for (f = tf + 1; f < 7; ++f) attacks |= (1ULL << (tr * 8 + f));
  for (f = tf - 1; f > 0; --f) attacks |= (1ULL << (tr * 8 + f));
  
  return attacks;
}
// init leaper pieces attacks
void init_leapers_attacks() {
  // loop over 64 board squares
  for (int square = 0; square < 64; ++square){
    // init pawn attacks
    pawn_attacks[white][square] = mask_pawn_attacks(white, square);
    pawn_attacks[black][square] = mask_pawn_attacks(black, square);

    // init knight attacks
    knight_attack[square] = mask_knight_attacks(square);

    // init king attacks
    king_attack[square] = mask_king_attacks(square);
  }
}


int main() {
  // U64 bitboard = mask_rook_attack(d4);
  // print_bitboard(bitboard);
  
  // init leaper pieces attacks
  // init_leapers_attacks(); 

  // loop over 64 board squares
  for (int square = 0; square < 64; ++square) {
    print_bitboard(mask_rook_attack(square));
  }
  return 0;
}