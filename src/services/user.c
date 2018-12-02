#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/account.h"

account *createUser(char *username, char *password, int point, int status) {
  account *user = malloc(sizeof(account));

  strcpy(user->username, username);
  strcpy(user->password, password);
  user->point = point;
  user->status = status;

  return user;
}