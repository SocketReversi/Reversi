#include <stdio.h>
#include <stdlib.h>

#define BLACK  1
#define NONE   0
#define WHITE -1

int** board;         
                 
int size;            
int current = BLACK; 
int count;          

void initialize (const int n);
void finalize();
void display();
int isReversible ( const int m, const int n );
int reverse( int m, int n );
int winner();
int play();