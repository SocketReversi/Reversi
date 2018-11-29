#define MAX_TABLE 2
#define EMPTY -1
#define WAITING 11
#define FULL 22

typedef struct tableGame{
	int master;
	int guest;
	int state;
	int id;
}tableGame;

void createTableList(tableGame table[MAX_TABLE]);
int createTable(int IDmaster,tableGame table[MAX_TABLE]);
int joinTable(int IDguest, tableGame table[MAX_TABLE]);
int findIDgamer(int IDgamer,tableGame table[MAX_TABLE]);
int leaveTable(int IDgamer, tableGame table[MAX_TABLE]);
int findID(int id, tableGame table[MAX_TABLE]);
void printTable(tableGame table[MAX_TABLE]);