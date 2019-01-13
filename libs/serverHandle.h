#define MAX_TABLE 2
#define EMPTY -1
#define WAITING 11
#define FULL 22
#define BACKLOG 20 /* Number of allowed connections */
#define BUFF_SIZE 1024
#define MASTER 111
#define GUEST  112

typedef struct ttable{

	int master; //chu
	int guest;	//khach
	int state;	//trang thai ban co
	
	int board[8][8]; //ban co
	int turn; //luot di cua nguoi choi
	int result; //ket qua tran co
	int current; //trang thai hien tai cua mau quan co
}table;

GSList *createTable(GSList *listTable, int id);
int joinTable(GSList *listTable, int id);
int Player(GSList *listTable, int id);

GSList *findWithID(GSList *listTable, int id);

GSList *leaveTable(GSList *listTable, int id);
void printTable(GSList *list);
void printListUser(GSList *list);
void copyBoard(int board1[8][8], int board2[8][8]);

account *find_User_Pass(GSList *list, Request *request);
account *find_User(GSList *list, Request *request);

int Register(GSList *listUser, Request *request);

Request *handleRequest(Request *request, GSList *listUser);
Request *groupClient(Request *request, GSList *listTable, int client);
Request *playGame(Request *request, GSList *listTable, int client);