typedef struct account{
	int id;
	int point;
	char username[30];
	char password[30];
	int state;
    int status;
} account;

account *createAccount(char *username, char *password, int point, int state, int status);
gint sortFunction(gconstpointer item1, gconstpointer item2);