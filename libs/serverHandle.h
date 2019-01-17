#define MAX_TABLE 2
#define EMPTY -1
#define WAITING 11
#define FULL 22
#define BACKLOG 20
#define BUFF_SIZE 1024
#define MASTER 111
#define GUEST  112

#define POINT 3

typedef struct ttable{

	int master; 
	int master_point;
	int master_rank;
	char master_name[50];

	int guest;
	int guest_point;
	int guest_rank;
	char guest_name[50];

	int state;
	
	int board[8][8]; 
	int turn;
	int result;
	int current;
}table;

GSList *createTable(GSList *listTable, GSList *listUser, int id, char name[50], int point);
int joinTable(GSList *listTable, GSList *listUser, int id, char name[50], int point);
int Player(GSList *listTable, int id);

GSList *findWithID(GSList *listTable, int id);

GSList *leaveTable(GSList *listTable, int id);
void printTable(GSList *list);
void printListUser(GSList *list);
void copyBoard(int board1[8][8], int board2[8][8]);

account *find_User_Pass(GSList *list, Request *request);
account *find_User(GSList *list, Request *request);
GSList *find(GSList *listUser, char name[50]);

int Register(GSList *listUser, Request *request);
int findIDPlayMate(GSList *listTable, int idPlayer);
int findPlayMate(GSList *listTable, int id, int client[FD_SETSIZE]);

Request *handleRequest(int state, Request *request, GSList *listUser, char user[50]);
Request *groupClient(int state, Request *request, GSList *listTable, GSList *listUser, int client, char user[50]);
Request *playGame(int state, Request *request, GSList *listTable, int client);