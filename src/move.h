#ifndef MOVE_H
#define MOVE_H

// is square attacked by the given side
int is_square_attacked(int square, int side);

void print_attacked_squares(int side);

// generate all moves
void generate_moves();

int encode_move(int source, int target, int piece, int promoted, int capture, int doublepush, int enpassant, int castling);

// extract source square
int get_move_source(int move);

// extract target square
int get_move_target(int move);

// extract piece
int get_move_piece(int move);

// extract promoted piece
int get_move_promoted(int move);

// extract capture flag
int get_move_capture(int move);

// extract double pawn push flag
int get_move_double(int move);

// extract enpassant flag
int get_move_enpassant(int move);

// extract castling flag
int get_move_castling(int move);

#endif