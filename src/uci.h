#ifndef UCI_H
#define UCI_H

// parse user/GUI move string input (eg. "e7e8q")
int parse_move(char* move_string);

// parse UCI "position" command
void parse_position(char* command);

// parse UCI "go" command
void parse_go(char* command);

// main UCI loop
void uci_loop();

#endif