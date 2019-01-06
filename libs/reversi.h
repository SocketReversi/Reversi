#include <stdio.h>
#include <stdlib.h>

#define BLACK  1
#define NONE   0
#define WHITE -1
#define SIZE 4
                   
int current = BLACK;        

void initialize (int board[SIZE][SIZE]);
void display(int board[SIZE][SIZE]);
int isReversible ( const int m, const int n , int board[SIZE][SIZE]);
int reverse( int m, int n , int board[SIZE][SIZE]);
int winner(int board[SIZE][SIZE]);
int play(int board[SIZE][SIZE]);