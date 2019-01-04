#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "../../libs/request.h"
#include "../../libs/account.h"
#include "../../libs/file.h"
#include "../../libs/server.h"

FILE *open(char *filename, char *action) {
	FILE *file = fopen(filename, action);

	if (file == NULL) {
		printf("File Can\'t Open\n");
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

  while (!feof(file)) {
    fscanf(file, "%s %s %d %d", username, password, &point, &status);

    account *user = createAccount(username, password, point, status);

    list = g_slist_append(list, user);
  }

  return list;
}