#define BACKLOG 20 /* Number of allowed connections */
#define BUFF_SIZE 1024

void printListUser(GSList *list);
account *find_User_Pass(GSList *list, Request *request);
account *find_User(GSList *list, Request *request);
Request *handleRequest(Request *request, GSList *listUser);