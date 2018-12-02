#define BACKLOG 20 /* Number of allowed connections */
#define BUFF_SIZE 1024
#define PATH_FILE "account.txt"

void paramsServerValid(int argc);
GSList *importUserToList();