#ifndef CONST_H
#define CONST_H

#include <stdint.h>
#define U64 uint64_t

// enum board squares
enum {
  a8, b8, c8, d8, e8, f8, g8, h8,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};

extern const char* square_to_coordinates[64];

// side to move (colors)
enum { white, black };

// mask for not ranks and files
extern const U64 NOT_AB_FILE;
extern const U64 NOT_A_FILE;
extern const U64 NOT_H_FILE;
extern const U64 NOT_GH_FILE;

// relevant occupancy bit count for every square on board for bishop
extern const int bishop_relevant_bits[64];
// relevant occupancy bit count for every square on board for rook
extern const int rook_relevant_bits[64];

enum { rook, bishop };

extern const U64 rook_magic_numbers[64];
extern const U64 bishop_magic_numbers[64];

// castling rights binary encoding

/*

    bin  dec
    
   0001    1  white king can castle to the king side
   0010    2  white king can castle to the queen side
   0100    4  black king can castle to the king side
   1000    8  black king can castle to the queen side

   examples

   1111       both sides an castle both directions
   1001       black king => queen side
              white king => king side

*/

enum { wk = 0b0001, wq = 0b0010, bk = 0b0100, bq = 0b1000 };

// encode pieces (capital letters = white, lowercase letters = black)
enum { P, N, B, R, Q, K, p, n, b, r, q, k};

// ASCII pieces
extern char ascii_pieces[12];

// convert ASCII character pieces to encode constants
extern int char_pieces[];

#endif