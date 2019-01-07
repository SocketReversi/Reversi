#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/request.h"
#include "../../libs/reversi.h"
#include "../../libs/clientHandle.h"

Request *clientHandle()
{
  int select; 
  Request *request = malloc(sizeof(Request));

  do {
    printf("1.LOGIN |2.REGISTER |3.LOGOUT |4.CREATE |5.JOIN |6.LEAVE |7.PLAY |8.MOVE ]\n");
    printf("Opcode: ");
    scanf("%d", &select);
  } while(select < LOGIN && select > MOVE);
  switch(select){

    case LOGIN: //1
      while(getchar()!='\n');
      printf("LOG IN\nUsername: ");
      fgets(request->username,50,stdin);
      printf("Password: ");
      fgets(request->password,50,stdin);
      request->username[strlen(request->username) - 1] = '\0';
      request->password[strlen(request->password) - 1] = '\0';
      break;

    case LOGOUT: //3
      while(getchar()!='\n');
      printf("LOG OUT\nUsername: ");
      fgets(request->username,50,stdin);
      request->username[strlen(request->username) - 1] = '\0';
      printf("%s\n", request->username);
      break;

    case REGISTER: //2
      while(getchar()!='\n');
      printf("REGISTER\nUsername: ");
      fgets(request->username,50,stdin);
      printf("Password: ");
      fgets(request->password,50,stdin);
      request->username[strlen(request->username) - 1] = '\0';
      request->password[strlen(request->password) - 1] = '\0';
      break;

    case CREATE:
      while(getchar()!='\n'); 
      break;

    case JOIN: //5
      while(getchar()!='\n');
      break;

    case LEAVE://6
      while(getchar()!='\n');
      break;

    case PLAY://7
      while(getchar()!='\n');
      break;

    case MOVE://8
      while(getchar()!='\n');
      int x,y;
      printf("DOC   :");scanf("%d",&x);
      printf("NGANG :");scanf("%d",&y);
      request->doc = x;
      request->ngang = y;
      break;

  }
  // Update request store send to server
  request->opcode = select;

  return request;
}

void renderMessage(Request *request) {
  printf("\n-----------\nMessage from server!\n");
  
  switch(request->opcode){

    case LOGIN_SUCCESS:
      printf("%s %s\n", request->message, request->username);
      break;

    case LOGOUT_SUCCESS:
      printf("%s %s\n", request->message, request->username);
      break;

    case PLAY_SUCCESS:
      display(request->board);
      break;

    case MOVE_SUCCESS:
      display(request->board);
      printf("%s\n", request->message);
      break;

    default:
      printf("%s\n", request->message);
      break;
  }

}