#include "attacks.h"
#include "util.h"

U64 pawn_attacks[2][64];
U64 knight_attack[64];
U64 king_attack[64];

// generate pawn attacks
U64 mask_pawn_attacks(int side, int square) {
  // result attacks bitboard
  U64 attacks = 0ULL;
    
  // piece bitboard
  U64 bitboard = 0ULL;

  // set piece on board
  set_bit(&bitboard, square);

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
  set_bit(&bitboard, square);

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
  set_bit(&bitboard, square);

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
U64 mask_rook_attacks(int square) {
  // return attack bitboard
  U64 attacks = 0ULL;

  // init ranks & files
  int r, f;

  // init target rank & files
  int tr = square / 8;
  int tf = square % 8;

  // mask relavent rook occupancy bits
  for (r = tr + 1; r < 7; ++r) attacks |= (1ULL << (r * 8 + tf));
  for (r = tr - 1; r > 0; --r) attacks |= (1ULL << (r * 8 + tf));
  for (f = tf + 1; f < 7; ++f) attacks |= (1ULL << (tr * 8 + f));
  for (f = tf - 1; f > 0; --f) attacks |= (1ULL << (tr * 8 + f));
  
  return attacks;
}

// generate bishop attacks on the fly
U64 bishop_attacks_on_the_fly(int square, U64 block) {
  // result attacks bitboard
  U64 attacks = 0ULL;

  // init ranks & files
  int r, f;

  // init target ranks & files
  int tr = square / 8;
  int tf = square % 8;

  // generate bishop attacks
  for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; ++r, ++f) {
    attacks |= (1ULL << (r * 8 + f));
    if ((1ULL << (r * 8 + f)) & block) break;
  }
  for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; --r, ++f) {
    attacks |= (1ULL << (r * 8 + f));
    if ((1ULL << (r * 8 + f)) & block) break;
  }
  for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; ++r, --f) {
    attacks |= (1ULL << (r * 8 + f));
    if ((1ULL << (r * 8 + f)) & block) break;
  }
  for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; --r, --f) {
    attacks |= (1ULL << (r * 8 + f));
    if ((1ULL << (r * 8 + f)) & block) break;
  }

  return attacks;
}

// generate rook attacks on the fly
U64 rook_attacks_on_the_fly(int square, U64 block) {
  // return attack bitboard
  U64 attacks = 0ULL;

  // init ranks & files
  int r, f;

  // init target rank & files
  int tr = square / 8;
  int tf = square % 8;

  // generate rook attacks
  for (r = tr + 1; r <= 7; ++r) {
    attacks |= (1ULL << (r * 8 + tf));
    if ((1ULL << (r * 8 + tf)) & block) break;
  }
  for (r = tr - 1; r >= 0; --r) {
    attacks |= (1ULL << (r * 8 + tf));
    if ((1ULL << (r * 8 + tf)) & block) break;
  }
  for (f = tf + 1; f <= 7; ++f) {
    attacks |= (1ULL << (tr * 8 + f));
    if ((1ULL << (tr * 8 + f)) & block) break;
  }
  for (f = tf - 1; f >= 0; --f) {
    attacks |= (1ULL << (tr * 8 + f));
    if ((1ULL << (tr * 8 + f)) & block) break;
  }
  
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

U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask) {
  // occupancy map
  U64 occupancy = 0ULL;

  // loop over the range of bits within attack mask
  for (int count = 0; count < bits_in_mask; ++count) {
    // get LS1B index of attack mask
    int square = get_ls1b_index(attack_mask);

    // pop LS1B in attack map
    pop_bit(&attack_mask, square);

    // make sure occupancy is on board
    if (index & (1 << count)) {
      // populate occupancy map 
      occupancy |= (1ULL << square);
    }
  }

  return occupancy;
}