#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "../../libs/request.h"
#include "../../libs/account.h"
#include "../../libs/file.h"
#include "../../libs/reversi.h"
#include "../../libs/serverHandle.h"

FILE *open(char *filename, char *action) {
	FILE *file = fopen(filename, action);

	if (file == NULL) {
		printf("File can not open\n");
		return NULL;
	} else {
		return file;
	}
}

GSList *importUserFromFileToList() {
  GSList *list = NULL;

  FILE *file = open(PATH_FILE, "r");

  if (file == NULL) {
    exit(0);
  }

  char username[30], password[30];
  int id, point, status;

  while(1) {
    fscanf(file, "%s %s %d %d", username, password, &point, &status);
    if(feof(file))
      break;
    account *user = createAccount(username, password, point, 0 , status);
    list = g_slist_append(list, user);
  }
  fclose(file);
  return list;
}

void updateData(GSList *list){
  FILE *f = open(PATH_FILE, "w");

  if (f == NULL) {
    exit(0);
  }
  GSList *var = list;
  while(var != NULL){
    account *acc = var->data;
    fprintf(f,"%s %s %d %d\n",acc->username,acc->password,acc->point,acc->status);
    var = var->next;
  }
  fclose(f);
}