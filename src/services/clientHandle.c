#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/request.h"
#include "../../libs/clientHandle.h"

Request *clientHandle()
{
  int select; 
  Request *request = malloc(sizeof(Request));

  do {
    printf("\n----------\nOpcode: ");
    scanf("%d", &select);
  } while(select < LOGIN && select > MOVE);
  switch(select){

    case LOGIN:
      while(getchar()!='\n');
      printf("LOG IN\nUsername: ");
      fgets(request->username,50,stdin);
      printf("Password: ");
      fgets(request->password,50,stdin);
      request->username[strlen(request->username) - 1] = '\0';
      request->password[strlen(request->password) - 1] = '\0';
      break;

    case LOGOUT:
      while(getchar()!='\n');
      printf("LOG OUT\nUsername: ");
      fgets(request->username,50,stdin);
      request->username[strlen(request->username) - 1] = '\0';
      printf("%s\n", request->username);
      break;

    case REGISTER:
      while(getchar()!='\n');
      printf("REGISTER\nUsername: ");
      fgets(request->username,50,stdin);
      printf("Password: ");
      fgets(request->password,50,stdin);
      request->username[strlen(request->username) - 1] = '\0';
      request->password[strlen(request->password) - 1] = '\0';
      break;

    case JOIN:
      break;

    case LEAVE:
      break;

    case MOVE:
      break;

  }
  // Update request store send to server
  request->opcode = select;

  return request;
}

void renderMessage(Request *request) {
  printf("\n-----------\nMessage from server!\n");
  printf("%s\n", request->message);
  //     break;
  // switch(request->opcode){

  //   case LOGIN_SUCCESS:
  //     printf("%s %s\n", request->message, request->username);
  //     break;

  //   case LOGIN_FAIL:
  //     printf("%s\n", request->message);
  //     break;

  //   case LOGOUT_SUCCESS:
  //     printf("%s %s\n", request->message, request->username);
  //     break;

  //   case LOGOUT_FAIL:
  //     printf("%s\n", request->message);
  //     break;

  //   case REGISTER_SUCCESS:
  //     printf
  // }

}