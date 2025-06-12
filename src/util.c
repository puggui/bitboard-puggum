#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "util.h"
#include "attacks.h"
#include "const.h"

// count bits within a bitboard 
int count_bits(U64 bitboard) {
  // bit counter
  int count = 0;
  
  // consecutively reset least significant 1st big
  while (bitboard) {
    // increament count
    count++;

    // reset least significant 1st bit
    bitboard &= bitboard - 1;
  }

  return count;
}

// get least significant 1st bit index
int get_ls1b_index(U64 bitboard) {
  // make sure bitboard is not 0
  if (bitboard) {
    // count trailing bits before LS1B
    return count_bits((bitboard & -bitboard) - 1);
  } else {
    // else return illegal index
    return -1;
  }
}

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

U64 get_bit(U64 bitboard, int square) {
 return (bitboard) & (1ULL << (square));
}

void set_bit(U64* bitboard, int square) {
  (*bitboard) |= (1ULL << (square));
}

void pop_bit(U64* bitboard, int square) {
  get_bit(*bitboard, square) ? (*bitboard) ^= (1ULL << (square)) : 0;
}

uint32_t random_state = 1804289383;

uint32_t get_random_u32_number() {
  // get current state
  unsigned int number = random_state;

  // XOR shift algorithm
  number ^= number << 13;
  number ^= number >> 17;
  number ^= number << 5;

  // update random number state
  random_state = number;

  return number;
}

U64 get_random_u64_number() {
  // define 4 random numbers 
  U64 n1, n2, n3, n4;

  // init random numbers slicing 16 bits from MS1B side
  n1 = (U64)((get_random_u32_number()) & 0xffff); 
  n2 = (U64)((get_random_u32_number()) & 0xffff); 
  n3 = (U64)((get_random_u32_number()) & 0xffff); 
  n4 = (U64)((get_random_u32_number()) & 0xffff); 

  return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

U64 generate_magic_number() {
  return get_random_u64_number() & get_random_u64_number() & get_random_u64_number();
}

U64 find_magic_number(int square, int relevant_bits, int bishop) {
  // init occupancies 
  U64 occupancies[4096];

  // init attack tables
  U64 attacks[4096];

  // init used attacks
  U64 used_attacks[4096];

  // init attack mask for a current piece
  U64 attack_mask = bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

  // init occupancy indicies
  int occupancy_indicies = 1 << relevant_bits;

  // loop over occupancy indicies
  for (int index = 0; index < occupancy_indicies; ++index) {
    // init occupancies
    occupancies[index] = set_occupancy(index, relevant_bits, attack_mask);

    // init attacks 
    if (bishop) {
      attacks[index] = bishop_attacks_on_the_fly(square, occupancies[index]);
    } else {
      attacks[index] = rook_attacks_on_the_fly(square, occupancies[index]);
    }
  }

  // test magic number loop
  for (int random_count = 0; random_count < 100000000; ++random_count) {
    // generate magic number candidate
    U64 magic_number = generate_magic_number();

    // skip unsuitable magic numbers
    if (count_bits((attack_mask * magic_number) & 0xff00000000000000) < 6) continue;

    // used to test for collision, set entire array to 0 every time a new square is computed 
    memset(used_attacks, 0ULL, sizeof(used_attacks));

    // init index & fail flag
    int index, fail;

    // test magic index loop
    for (index = 0, fail = 0; !fail && index < occupancy_indicies; ++index) {
      // init magic index
      int magic_index = (int)((occupancies[index] * magic_number) >> (64 - relevant_bits));

      // if magic index works
      if (used_attacks[magic_index] == 0ULL) {
        // init used attacks 
        used_attacks[magic_index] = attacks[index];
      } else if (used_attacks[magic_index] != attacks[index]) {
        // magic index doesn't work 
        fail = 1;
      }
    }
    // if magic number works
    if (!fail) {
      return magic_number;
    }
  }

  // if magic number doesn't work
  printf("Magic number fails");

  return 0ULL;
}

void init_magic_number() {
  // rook magic numbers
  for (int square = 0; square < 64; ++square) {
    // init rook magic numbers
    printf("0x%llxULL,\n", find_magic_number(square, rook_relevant_bits[square], rook));
  }
  printf("\n");
  // bishop magic numbers
  for (int square = 0; square < 64; ++square) {
    // init bishop magic numbers
    printf("0x%llxULL,\n", find_magic_number(square, bishop_relevant_bits[square], bishop));
  }
}

void init_all() {
  init_leapers_attacks();
  // init_magic_number();
  init_slider_attack(bishop);
  init_slider_attack(rook);
}

U64 bitboards[6];
U64 occupancies[2];
int side;
int enpassent = no_sq;
int castle;

void print_board() {
  printf("\n");
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      // init square 
      int square = rank * 8 + file;
      
      // print ranks
      if (!file) {
        printf("%d  ", 8 - rank);
      }
      // define piece variable 
      int piece = -1;

      // loop over all piece bitboard
      for (int bb_piece = P; bb_piece <=K; ++bb_piece) {
        if (get_bit(bitboards[bb_piece] & occupancies[white], square)) {
          piece = bb_piece;
        } 
        if (get_bit(bitboards[bb_piece] & occupancies[black], square)) {
          piece = bb_piece + 6;
        } 
      }
      
      // 
      printf("%c ", piece == -1 ? '.' : ascii_pieces[piece]);
    }
    printf("\n");
  }
  // print files
  printf("\n   a b c d e f g h\n\n");

  // print side to move
  printf("   side:    %s\n\n", side ? "black" : "white");

  // print en passant square
  printf("   en passant: %s\n\n", (enpassent != no_sq) ? square_to_coordinates[enpassent] : "no");

  // print castling rights
  printf("   castling: %c%c%c%c\n\n", (castle & wk) ? 'K' : '-', (castle & wq) ? 'Q' : '-', (castle & bk) ? 'k' : '-', (castle & bq) ? 'q' : '-');
}

void add_piece(int piece, int index) {
  // calculate value on bitboard
  U64 square = 1ULL << index;

  // if piece is white value 0-5
  if (piece >= 0 && piece <= 5) {
    bitboards[piece] |= square;
    occupancies[white] |= square;
  } 
  // if black piece value 6-11 
  else if (piece >= 6 && piece <= 11) {
    bitboards[piece - 6] |= square;
    occupancies[black] |= square;
  } 
}

void parse_fen(char* fen) {
  // reset board position and occupancies (bitboards) 
  memset(bitboards, 0ULL, sizeof(bitboards));
  memset(occupancies, 0ULL, sizeof(occupancies));

  // reset game state variable
  side = 0;
  castle = 0;
  enpassent = no_sq;

  // init FEN string copy 89 + 1 for '\0' is the longest possible FEN string
  char fen_copy[90]; 
  
  // copy FEN string to not modify original
  strcpy(fen_copy, fen);

  // tokenize FEN string to array <piece placement> <side to move> <castling rights> <en passent> <halfmove> <fullmove>
  char* token[6]; 

  // index to count FEN's 6 field
  int i = 0;

  char* part = strtok(fen_copy, " ");
  while (part != NULL && i < 6) {
    token[i] = part;
    part = strtok(NULL, " ");
    i++;
  }

  // piece placement field
  char* ptr = token[0];
  // init index to count the 64 tiles
  int index = 0;
  // parse <piece placement>
  while (index < 64) {
    if (isalpha(*ptr) && index < 64) {
      // init piece type
      int piece = char_pieces[*ptr];
      
      // set piece on correspondig bitboard
      add_piece(piece, index);

      // increment count 64
      index++;
    }
    
    // match empty square numbers within FEN string
    else if (isdigit(*ptr) && index < 64) {
      // intit offset (convert char 0 to int 0);
      int offset = *ptr - '0';

      // increment counter by number
      index += offset;
    }

    ptr++;
  }

  // parse <side to move>
  if (strcmp(token[1], "w") == 0) {
    side = white;
  } else if (strcmp(token[1], "b") == 0) {
    side = black;
  }

  // parse <castling right>
  for (int i = 0; token[2][i] != '\0'; i++) {
    switch (token[2][i]) {
      case 'K': castle |= 0b0001; break;
      case 'Q': castle |= 0b0010; break;
      case 'k': castle |= 0b0100; break;
      case 'q': castle |= 0b1000; break;
    }
  }

  // parse <en passent square>
  if (token[3][0] != '-') {
    int file = token[3][0] - 'a';        // 'a'–'h' → 0–7
    int rank = token[3][1] - '1';        // '1'–'8' → 0–7 (from bottom)
    enpassent = (7 - rank) * 8 + file;   // convert to 0–63 index (top-down)
  }

  // halfmove = atoi(token[4]);
  // fullmove = atoi(token[5]);
}