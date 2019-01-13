#include "../../libs/reversi.h"
#include <stdio.h>
#include <stdlib.h>

void initialize (int board[SIZE][SIZE]) {
	
	int i, j;
	for ( i=0; i < SIZE; i++ ) {
		for ( j=0; j < SIZE; j++ ) {
			board[i][j] = NONE;
		}
	}
	board[SIZE/2-1][SIZE/2-1] = WHITE;
	board[SIZE/2-1][SIZE/2]   = BLACK;
	board[SIZE/2][SIZE/2-1]   = BLACK;
	board[SIZE/2][SIZE/2]     = WHITE;
}


void display (int board[SIZE][SIZE]) {
	
	
	int i, j;
	
	printf("\n ");
	for ( i=0; i < SIZE; i++ ) printf("%4d", i);
	printf("\n");
	
	printf("  ┌");
	for ( i=0; i < SIZE-1; i++ ) { printf("---┬"); }
	printf("---┐\n");
	
	for ( i=0; i < SIZE; i++ ) {
		printf("%2d|", i);
		for ( j=0; j < SIZE; j++ ) {
			switch ( board[j][i] ) {
				case BLACK: printf(" ○ ");
				            break;
				case WHITE: printf(" ● ");
				            break;
				case NONE : printf("　 ");
				            break;
			}
			printf("|");
		}
		printf("\n");
		if ( i < SIZE-1 ) {
			printf("  ├");
			for ( j=0; j < SIZE-1; j++ ) {
				printf("---+");
			}
			printf("---┤\n");
		}
	}

	printf("  └");
	for ( i=0; i < SIZE-1; i++ ) { printf("---┴"); }
	printf("---┘\n");
}

//Hien thi ban co-------------------------------//
// void display (int board[SIZE][SIZE]) {
// 	int i, j;
// 	printf("\n ");
// 	for ( i=0; i < SIZE; i++ ) printf(" %d", i);
// 	printf("\n");
// 	for ( i=0; i < SIZE; i++ ) {
// 		printf("%d", i);
// 		for ( j=0; j < SIZE; j++ ) {
// 			switch ( board[j][i] ) {
// 				case BLACK: printf(" ○");
// 				            break;
// 				case WHITE: printf(" ●");
// 				            break;
// 				case NONE:	printf(" -");
// 							break;
// 				default: 
// 					board[j][i] = NONE;
// 					printf(" +");
// 				    break;
// 			}
// 		}
// 		printf("\n");
// 	}
// }


//Kiem tra co the dao nguoc duoc quan co--------------//
int isReversible ( const int m, const int n,int board[SIZE][SIZE],int current ) {
	int i, j;
	int result = 0; 
	int count = 0;  
	
	if ( m < 0 || SIZE <= m || n < 0 || SIZE <= n ) return 0;
	if ( board[m][n] != NONE ) return 0;
	
	
	if ( n > 1 && board[m][n-1] == -current ) {  		for ( j=n-2; j >= 0; j-- ) {
			if ( board[m][j] == current ) { result += 1; count += n-j-1; break; }
			if ( board[m][j] == NONE    ) { break; }
		}
	}
	

	if ( n < SIZE-2 && board[m][n+1] == -current ) {  
		for ( j=n+2; j < SIZE; j++ ) {
			if ( board[m][j] == current ) { result += 2; count += j-n-1; break; }
			if ( board[m][j] == NONE    ) { break; }
		}
	}
	
		if ( m > 1 && board[m-1][n] == -current ) {  		for ( i=m-2; i >= 0; i-- ) {
			if ( board[i][n] == current ) { result += 4; count += m-i-1; break; }
			if ( board[i][n] == NONE    ) { break; }
		}
	}

	
	if ( m < SIZE-2 && board[m+1][n] == -current ) {  
		for ( i=m+2; i < SIZE; i++ ) {
			if ( board[i][n] == current ) { result += 8; count += i-m-1; break; }
			if ( board[i][n] == NONE    ) { break; }
		}
	}


	if ( m > 1 && n > 1 && board[m-1][n-1] == -current ) {  
		for ( i=m-2, j=n-2;  i>=0 && j>=0;  i--, j-- ) {
			if ( board[i][j] == current ) { result += 16; count += n-j-1; break; }
			if ( board[i][j] == NONE    ) { break; }
		}
	}
	

	if ( m < SIZE-2 && n > 1 && board[m+1][n-1] == -current ) {  		for ( i=m+2, j=n-2;  i<SIZE && j>=0;  i++, j-- ) {
			if ( board[i][j] == current ) { result += 32; count += n-j-1; break; }
			if ( board[i][j] == NONE    ) { break; }
		}
	}


	if ( m > 1 && n < SIZE-2 && board[m-1][n+1] == -current ) {  		for ( i=m-2, j=n+2;  i>=0 && j<SIZE;  i--, j++ ) {
			if ( board[i][j] == current ) { result += 64; count += j-n-1; break; }
			if ( board[i][j] == NONE    ) { break; }
		}
	}
	
		if ( m < SIZE-2 && n < SIZE-2 && board[m+1][n+1] == -current ) {  		for ( i=m+2, j=n+2;  i < SIZE && j < SIZE;  i++, j++ ) {
			if ( board[i][j] == current ) { result += 128; count += j-n-1; break; }
			if ( board[i][j] == NONE    ) { break; }
		}
	}


	return result;
}


//Thuc hien dao nguoc quan co--------------------//
value reverse ( int m, int n , int board[SIZE][SIZE],int current) {
	int i, j;
	int reversed = 0;
	value message; //luu tru gia tri tra ve cho ham

	int reversible = isReversible(m, n, board,current);

	if ( !reversible ){
		message.state = 0;
		message.color = current;
		return message;
	}

	board[m][n] = current;


	
	if ( reversible & 1 ) {
		for ( j=n-1;  board[m][j] == -current;  j-- ) {
			board[m][j] = current;
			reversed++;
		}
	}
	reversible >>= 1;
	
		if ( reversible & 1 ) {
		for ( j=n+1;  board[m][j] == -current;  j++ ) {
			board[m][j] = current;
			reversed++;
		}
	}
	reversible >>= 1;
	

	if ( reversible & 1 ) {
		for ( i=m-1;  board[i][n] == -current;  i-- ) {
			board[i][n] = current;
			reversed++;
		}
	}
	reversible >>= 1;


	if ( reversible & 1 ) {
		for ( i=m+1;  board[i][n] == -current;  i++ ) {
			board[i][n] = current;
			reversed++;
		}
	}
	reversible >>= 1;

	
	if ( reversible & 1 ) {
		for ( i=m-1, j=n-1;  board[i][j] == -current;  i--, j-- ) {
			board[i][j] = current;
			reversed++;
		}
	}
	reversible >>= 1;
	
	
	if ( reversible & 1 ) {
		for ( i=m+1, j=n-1;  board[i][j] == -current;  i++, j-- ) {
			board[i][j] = current;
			reversed++;
		}
	}
	reversible >>= 1;


	if ( reversible & 1 ) {
		for ( i=m-1, j=n+1;  board[i][j] == -current;  i--, j++ ) {
			board[i][j] = current;
			reversed++;
		}
	}
	reversible >>= 1;
	

	if ( reversible & 1 ) {
		for ( i=m+1, j=n+1;  board[i][j] == -current;  i++, j++ ) {
			board[i][j] = current;
			reversed++;
		}
	}
	reversible >>= 1;

	current = -current;
	

	for ( i=0; i < SIZE; i++ ) {
		for ( j=0; j < SIZE; j++ ) {
			if ( isReversible(i, j, board, current) ) { 
				message.state = reversed;
				message.color = current;

				return message;          			
			}
		}
	}


	current = -current;
	for ( i=0; i < SIZE; i++ ) {
		for ( j=0; j < SIZE; j++ ) {
			if ( isReversible(i, j, board,current) ) {
				message.state = reversed;
				message.color = current;

				return message;
			}
		}
	}
	
	message.state = -1;
	message.color = current;

	return message;
}

//Xac dinh ben nao chien thang-------------------------------------------//
int winner (int board[SIZE][SIZE] ) {
	int i, j;
	int t = 0, count=0, blank=0;
	
	for ( i=0; i < SIZE; i++ ) {
		for ( j=0; j < SIZE; j++ ) {
			t += board[i][j];
			if(board[i][j]==BLACK) count++;
			else if(board[i][j]==NONE) blank++;
		}
	}
	printf("Black %d  vs White %d \n", count, SIZE*SIZE-count-blank);
	
	if      ( t < 0 ) return WHITE;
	else if ( t > 0 ) return BLACK;
	else              return NONE;
}

//choi game-------------------------------------------//
int play(int board[SIZE][SIZE]){
	int m, n;  	 
	int result = 1;  
	int current = BLACK;

	value message;
	
	//khoi tao ban
	initialize(board);    
	//bat dau choi 
	while (result != 0) {
		display(board);

		if ( current == BLACK ) {
			printf("Black turn.\n");
		}
		else {
			printf("White turn.\n");
		}
		while ( 1 ) {
			printf("Nhap nuoc co (Doc - Ngang).\n");
			printf("Doc  :");scanf("%d",&m);
			printf("Ngang:");scanf("%d",&n);
			
			message = reverse(m, n, board,current);
			result = message.state;
			current = message.color;
			printf("\n[Trang thai tro choi]-[%d]\n",result);
			printf("Gia tri cua [current]-[%d]\n",current);
			if ( result == 0 ) {
				printf("(%d,%d) you can't place a stone here。\n", m, n);
				printf("place again、");
			}
			else {
				break;
			}
		}

	}
	printf("Result: %d\n",result);
	display(board);
	printf("\nresult: ");
	switch ( winner(board) ) {
		case BLACK: printf("winner, black!\n\n");
			        break;
		case WHITE: printf("winner, white!\n\n");
			        break;
		case NONE : printf("no winner!\n\n");
			        break;
	}
	return 0;
}

// int main(){
// 	int board[SIZE][SIZE];
// 	play(board);
// 	return 0;
// }