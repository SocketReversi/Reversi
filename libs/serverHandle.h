#define MAX_TABLE 2
#define EMPTY -1
#define WAITING 11
#define FULL 22
#define BACKLOG 20 /* Number of allowed connections */
#define BUFF_SIZE 1024

typedef struct tableGame {
	int board[8][8];
	int master;
	int guest;
	int state;
	int id;
} tableGame;

void createTableList(tableGame table[MAX_TABLE]);
int createTable(int IDmaster,tableGame table[MAX_TABLE]);
int joinTable(int IDguest, tableGame table[MAX_TABLE]);
int findIDgamer(int IDgamer,tableGame table[MAX_TABLE]);
int leaveTable(int IDgamer, tableGame table[MAX_TABLE]);
int findID(int id, tableGame table[MAX_TABLE]);
void printTable(tableGame table[MAX_TABLE]);

void printListUser(GSList *list);
account *find_User_Pass(GSList *list, Request *request);
account *find_User(GSList *list, Request *request);
Request *handleRequest(Request *request, GSList *listUser);
Request *groupClient(Request *request, tableGame table[MAX_TABLE], int client);