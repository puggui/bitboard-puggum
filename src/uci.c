#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uci.h"
#include "const.h"
#include "move.h"
#include "util.h"
#include "search.h"

#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// parse user/GUI move string input (eg. "e7e8q")
int parse_move(char* move_string) {
  // create move list instance
  moves move_list[1];

  // generate moves
  generate_moves(move_list);

  // parse source and target square (file + rank * 8)
  int source_square = (move_string[0] - 'a') + (8 - (move_string[1] - '0')) * 8;
  int target_square = (move_string[2] - 'a') + (8 - (move_string[3] - '0')) * 8;

  // loop over moves withina move list
  for (int move_count = 0; move_count < move_list->count; ++move_count) {
    // init move
    int move = move_list->moves[move_count];

    // make sure source & target squares are availabale within the generated move
    if (source_square == get_move_source(move) && target_square == get_move_target(move)) {
      // init promoted piece
      int promoted_piece = get_move_promoted(move);
      
      // promoted piece is available
      if (promoted_piece) {
        if ((promoted_piece == Q || promoted_piece == q) && move_string[4] == 'q') return move;
        if ((promoted_piece == R || promoted_piece == r) && move_string[4] == 'r') return move;
        if ((promoted_piece == B || promoted_piece == b) && move_string[4] == 'b') return move;
        if ((promoted_piece == N || promoted_piece == n) && move_string[4] == 'n') return move;
        // continue the loop on possible wrong promotions (eg. e7e8f)
        continue;
      }
      // return legal move
      return move;
    } 
  }
  // return illegal move
  return 0;
}

void parse_position(char* command) {
  // shift pointer to beginning of next token
  command += 9;

  // init pointer to the current character in the command string
  char *current_char = command;

  // parse UCI "startpos" command
  if (strncmp(command, "startpos", 8) == 0) {
    // init chess board with start position
    parse_fen(start_position);
  } else { // parse UCI "fen" command
    current_char = strstr(command, "fen");

    // if no "fen" command is available within command string
    if (current_char == NULL) {
      parse_fen(start_position);
    } else { // found "fen" substring
      // shift pointer to the right where next token begins
      current_char += 4;

      // init chess board with posisiton from FEN string
      parse_fen(current_char);
    }
  }
  // parse moves after position
  current_char = strstr(command, "moves");

  // moves available
  if (current_char != NULL) {
    // shift pointer to the right where next token begins
    current_char += 6;

    // loop over moves within a move string
    while (*current_char) {
      // parse next move
      int move = parse_move(current_char);

      // if no more moves
      if (move == 0) break;

      // make move on the chess board
      make_move(move, all_moves);

      // move current character pointer to the end of current move
      while(*current_char && *current_char != ' ') current_char++;

      // go to next move
      current_char++;
    }
  }
  // print board
  // print_board();
}

void parse_go(char* command) {
  // init depth 
  int depth = -1;

  // init char pointer to current depth argument
  char* current_depth = strstr(command, "depth");

  // hadle fixed depth search
  if (current_depth) {
    // convert string to int and assign resulting value to depth
    depth = atoi(current_depth + 6);
  } else {
    // different time control placeholder
    depth = 6;
  }

  // search position
  search_position(depth);
}

void uci_loop() {
  // reset stdin & stdout buffers
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  
  // define user / GUI input buffer
  char input[2000];

  // print engine info 
  printf("id name pUgi\n");
  printf("id name puggies\n");
  printf("uciok\n");

  // main loop
  while (1) {
    // reset user / GUI input
    memset(input, 0, sizeof(input));

    // make sure output reaches the GUI
    fflush(stdout);

    // get user / GUI input
    if (!fgets(input, 2000, stdin)) continue;

    // make sure input is available 
    if (input[0] == '\n') continue;

    // parse UCI "isready" command
    if (strncmp(input, "isready", 7) == 0) {
      printf("readyok\n");
      continue;
    }

    // parse UCI "position" command
    if (strncmp(input, "position", 8) == 0) {
      // call parse position function
      parse_position(input);
    }

    // parse UCI "ucinewgame" command
    else if (strncmp(input, "ucinewgame", 10) == 0) {
      // calll parse position function
      parse_position("position startpos");
    }

    // parse UCI "go" command
    else if (strncmp(input, "go", 2) == 0) {
      parse_go(input);
    }

    // parse UCI "quit" command
    else if (strncmp(input, "quit", 4) == 0) {
      // quit chess engine program execution
      break;
    }

    else if (strncmp(input, "uci", 3) == 0) {
      // print engine info
      printf("id name pUgi\n");
      printf("id name puggies\n");
      printf("uciok\n");
    }
  }
}