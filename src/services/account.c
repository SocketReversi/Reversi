#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "../../libs/account.h"

//create accout with information---------------------//
account *createAccount(char *username, char *password, int point, int state, int status) {
  account *user = malloc(sizeof(account));

  strcpy(user->username, username);
  strcpy(user->password, password);
  user->point = point;
  user->state = state;
  user->status = status;

  return user;
}

//function to sort list -----------------------------------//
gint sortFunction(gconstpointer item1, gconstpointer item2) {
  account *first = (account *)item1;
  account *second = (account *)item2;
  return first->point < second->point;
}