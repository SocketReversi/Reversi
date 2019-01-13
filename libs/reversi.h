#define BLACK  1
#define NONE   0
#define WHITE -1
#define SIZE 8
           
typedef struct value{
	int state;
	int color;

}value;

//int current = BLACK;        

void initialize (int board[SIZE][SIZE]);
void display(int board[SIZE][SIZE]);
int isReversible ( const int m, const int n , int board[SIZE][SIZE], int current);
value reverse( int m, int n , int board[SIZE][SIZE], int current);
int winner(int board[SIZE][SIZE]);
int play(int board[SIZE][SIZE]);