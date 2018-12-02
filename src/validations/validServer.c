#include <stdio.h>
#include <stdlib.h>

void paramsServerValid(int argc) {
  if (argc < 2)
  {
    printf("Parameter invalid\n");
    exit(0);
  }
}