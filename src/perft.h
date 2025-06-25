
#ifndef PERFT_H 
#define PERFT_H

// leaf nodes (number of positions reach during the test of the move generator at a given depth)
extern long nodes;

// test variables
extern long captures;
extern long ep;
extern long castles;
extern long promotions;
extern long checks;

void perft_driver(int depth); 

void perft_test(int depth);

#endif 