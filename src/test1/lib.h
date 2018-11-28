#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//state for game
#define MAX_TABLE 10
#define TRONG -1
#define CO1NGUOI 1
#define CO2NGUOI 2

typedef struct tableGame{
	int master;
	int guest;
	int state;
	int IDtable;
}tableGame;

void createTableList(tableGame table[MAX_TABLE]);
int createTable(int IDmaster,tableGame table[MAX_TABLE]);
int joinTable(int IDguest, tableGame table[MAX_TABLE]);
int findIDgamer(int IDgamer,tableGame table[MAX_TABLE]);