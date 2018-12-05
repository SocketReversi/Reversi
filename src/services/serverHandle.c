#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/request.h"
#include "../../libs/server.h"
#include "../../libs/valid.h"

void setMessageEmptyError(Request *sendRequest) {
  free(sendRequest);
  sendRequest = setRequest(EMPTY, "Message is empty");
}

Request *handleRequest(Request *request)
{
  Request *sendRequest = malloc(sizeof(Request));

  memset(sendRequest->message, '\0', MAX_MESSAGE);

  switch (request->opcode)
  {
  case LOGIN:
    sendRequest->opcode = SUCCESS;
    strcpy(sendRequest->message, "Success");
    break;
  case USER_LOGIN:
    if (isEmpty(request->message)) {
      setMessageEmptyError(sendRequest);
      break;
    }
    sendRequest->opcode = CAN_PASS_LOGIN;
    strcpy(sendRequest->message, "You can input password to login");
    break;
  case PASS_LOGIN:
    if (isEmpty(request->message)) {
      setMessageEmptyError(sendRequest);
      break;
    }
    sendRequest->opcode = LOGIN_SUCCESS;
    strcpy(sendRequest->message, "You login successfull. You can play game reversi");
    break;
  case REGISTER:
    sendRequest->opcode = SUCCESS;
    strcpy(sendRequest->message, "Success");
    break;
  case USER_REGISTER:
    if (isEmpty(request->message)) {
      setMessageEmptyError(sendRequest);
      break;
    }
    sendRequest->opcode = CAN_PASS_REGISTER;
    strcpy(sendRequest->message, "You can input password to register");
    break;
  case PASS_REGISTER:
    if (isEmpty(request->message)) {
      setMessageEmptyError(sendRequest);
      break;
    }
    sendRequest->opcode = REGISTER_SUCCESS;
    strcpy(sendRequest->message, "You register successfull. You can login to system");
    break;
  case LOGOUT:
    if (isEmpty(request->message)) {
      setMessageEmptyError(sendRequest);
      break;
    }
    sendRequest->opcode = LOGOUT_SUCCESS;
    strcpy(sendRequest->message, "You log out successfull");
    break;
  default:
    break;
  }

  return sendRequest;
}
