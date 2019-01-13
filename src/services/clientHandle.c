#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/request.h"
#include "../../libs/reversi.h"
#include "../../libs/clientHandle.h"

Request *clientHandle()
{
  int select; 
  char input[10];
  Request *request = malloc(sizeof(Request));

  do {
    printf("\n");
    printf("Opcode: ");
    scanf("%s", input);
    select = atoi(input);
  } while(select < LOGIN || select > MOVE);
  printf("OPCODE cua client : %d\n",select);
  switch(select){

    case LOGIN: //1

      printf("LOG IN\nUsername: ");
      scanf("%s",request->username);
      printf("Password: ");
      scanf("%s",request->password);
      break;

    case LOGOUT: //3

      printf("LOG OUT\nUsername: ");
      scanf("%s",request->username);
      printf("%s\n", request->username);
      break;

    case REGISTER: //2

      printf("REGISTER\nUsername: ");
      scanf("%s",request->username);
      printf("Password: ");
      scanf("%s",request->password);
      break;

    case CREATE://4
      break;

    case JOIN: //5
      break;

    case CHECK://6
      break;

    case CHAT: //7
      printf("CHAT\nMessage: ");
      scanf("%s",request->message);
      break;

    case LEAVE://8
      break;

    case PLAY://9
      break;

    case MOVE://10

      printf("MOVE\nDOC  :");
      scanf("%d",&request->doc);
      printf("NGANG: ");
      scanf("%d",&request->ngang);
      while(request->doc < 0 || request->doc >7 || request->ngang < 0 || request->ngang >7){
        
        printf("Nuoc co khong hop le! Nhap lai\n");
        printf("MOVE\nDOC  :");
        scanf("%d",&request->doc);
        printf("NGANG: ");
        scanf("%d",&request->ngang);
      }
      break;

    default:
      break;

  }
  // Update request store send to server
  request->opcode = select;

  return request;
}

int renderMessage(Request *request) {
  printf("\n-------------------\nMessage -> ");
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

    case MOVE_FAIL:
      display(request->board);
      printf("%s\n", request->message);
      break;

    case CREATE_SUCCESS:
      printf("TB: %s\n", request->message);
      return 1;

    default:
      printf("%s\n", request->message);
      break;
  }

  return 0; //khong co yeu cau doi them
}