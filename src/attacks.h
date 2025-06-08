#ifndef ATTACKS_H
#define ATTACKS_H

#include "const.h"

// pawn attack table [side][square]
extern U64 pawn_attacks[2][64];

// knight attack table [square]
extern U64 knight_attack[64];

// king attack table [square]
extern U64 king_attack[64];

// generate leaper piece attack
U64 mask_pawn_attacks(int side, int square);
U64 mask_knight_attacks(int square);
U64 mask_king_attacks(int square);

// generate sliding piece attack
U64 mask_bishop_attacks(int square);
U64 mask_rook_attack(int square);
U64 bishop_attacks_on_the_fly(int square, U64 block);
U64 rook_attacks_on_the_fly(int square, U64 block);

void init_leapers_attacks();

#endif