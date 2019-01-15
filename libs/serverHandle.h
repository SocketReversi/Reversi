#define MAX_TABLE 2
#define EMPTY -1
#define WAITING 11
#define FULL 22
#define BACKLOG 20 /* Number of allowed connections */
#define BUFF_SIZE 1024
#define MASTER 111
#define GUEST  112

#define POINT 100 //So diem + or - sau moi van co

typedef struct ttable{

	int master; //chu
	char master_name[50];
	int guest;	//khach
	char guest_name[50];
	int state;	//trang thai ban co
	
	int board[8][8]; //ban co
	int turn; //luot di cua nguoi choi
	int result; //ket qua tran co
	int current; //trang thai hien tai cua mau quan co
}table;

GSList *createTable(GSList *listTable, int id, char name[50]);
int joinTable(GSList *listTable, int id, char name[50]);
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
Request *groupClient(int state, Request *request, GSList *listTable, int client, char user[50]);
Request *playGame(int state, Request *request, GSList *listTable, int client);