#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/account.h"

void paramsClientValid(int argc) {
  if (argc < 3)
  {
    printf("Parameter invalid\n");
    exit(0);
  }
}

void paramsServerValid(int argc) {
  if (argc < 2)
  {
    printf("Parameter invalid\n");
    exit(0);
  }
}

int isEmpty(char *text) {
  if (strcmp(text, "") == 0) {
    return 1;
  }
  return 0;
}