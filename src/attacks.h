#ifndef ATTACKS_H
#define ATTACKS_H

#include "const.h"

// pawn attack table [side][square]
extern U64 pawn_attacks[2][64];

// knight attack table [square]
extern U64 knight_attack[64];

// king attack table [square]
extern U64 king_attack[64];

// bishop attack masks
extern U64 bishop_masks[64];

// rook attack masks
extern U64 rook_masks[64];

// bishop attack table [square][occupancies] 2^9 max possible combination
extern U64 bishop_attacks[64][1 << 9];

// rook attack table [square][occupancies] 2^12 max possible combination
extern U64 rook_attacks[64][1 << 12];

// generate leaper piece attack
U64 mask_pawn_attacks(int side, int square);
U64 mask_knight_attacks(int square);
U64 mask_king_attacks(int square);

// generate sliding piece attack
U64 mask_bishop_attacks(int square);
U64 mask_rook_attacks(int square);
U64 bishop_attacks_on_the_fly(int square, U64 block);
U64 rook_attacks_on_the_fly(int square, U64 block);

// init attack table for leaper pieces
void init_leapers_attacks();

// init attack table for slider pieces
void init_slider_attack(int bishop);

// create blocker based on attack mask
U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask);

U64 get_bishop_attacks(int square, U64 occupancy);
U64 get_rook_attacks(int square, U64 occupancy);

#endif