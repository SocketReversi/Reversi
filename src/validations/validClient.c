#include <stdio.h>
#include <stdlib.h>

void paramsClientValid(int argc) {
  if (argc < 3)
  {
    printf("Parameter invalid\n");
    exit(0);
  }
}