typedef struct account{
	int id;
	int point;
	char username[30];
	char password[30];
  int status;
} account;

account *createAccount(char *username, char *password, int point, int status);
account *accountIsExistByUsername(char *username);