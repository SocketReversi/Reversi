#define PATH_FILE "account.txt"

FILE *open(char *filename, char *action);
GSList *importUserFromFileToList();
void updateData(GSList *list);