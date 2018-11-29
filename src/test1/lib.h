#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//state for game
#define MAX_TABLE 2
#define TRONG -1
#define CO1NGUOI 11
#define CO2NGUOI 22

typedef struct tableGame{
	int master;
	int guest;
	int state;
	int IDtable;
}tableGame;

typedef struct account{
	int IDacc;
	int point;
	int rank;
	char username[30];
	char password[30];
}acc;

void createTableList(tableGame table[MAX_TABLE]);
int createTable(int IDmaster,tableGame table[MAX_TABLE]);
int joinTable(int IDguest, tableGame table[MAX_TABLE]);
int findIDgamer(int IDgamer,tableGame table[MAX_TABLE]);
void printInfoAccount(acc account);
int leaveTable(int IDgamer, tableGame table[MAX_TABLE]);
int findID(int id, tableGame table[MAX_TABLE]);
void printTable(tableGame table[MAX_TABLE]);