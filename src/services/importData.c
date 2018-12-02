#include <gtk/gtk.h>
#include <stdlib.h>
#include "../../libs/account.h"
#include "../../libs/file.h"
#include "../../libs/server.h"

GSList *importUserToList() {
  GSList *list = NULL;

  FILE *file = open(PATH_FILE, "r");

  if (file == NULL) {
    exit(0);
  }

  char username[30], password[30];
  int id, point, status;

  while (!feof(file)) {
    fscanf(file, "%s %s %d %d", username, password, &point, &status);

    account *user = createUser(username, password, point, status);

    list = g_slist_append(list, user);
  }

  return list;
}



