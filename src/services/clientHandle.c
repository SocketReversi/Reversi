#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/request.h"

Request *clientHandle()
{
  int select; 
  Request *request = malloc(sizeof(Request));

  do {
    printf("Opcode: ");
    scanf("%d", &select);
  } while(select < LOGIN && select > MOVE);

  // Client input message send to server  
  printf("Message: ");
  gets(request->message);

  // Update request store send to server
  request->message[strlen(request->message) - 1] = '\0';
  request->opcode = select;

  return request;
}