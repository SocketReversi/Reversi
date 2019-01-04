#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/request.h"
#include "../../libs/server.h"

Request *handleRequest(Request *request)
{
  Request *sendRequest = malloc(sizeof(Request));

  memset(request->message, '\0', 50);

  switch (request->opcode)
  {
  case LOGIN:
    sendRequest->opcode = SUCCESS;
    strcpy(sendRequest->message, "Thanh cong");
    break;
  default:
    break;
  }

  return sendRequest;
}