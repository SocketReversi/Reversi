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
    printf("\n\n");
    printf("Opcode: ");
    scanf("%d", &select);
  } while(select < LOGIN || select > MOVE);
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

    case LEAVE://6
      break;

    case PLAY://7
      break;

    case MOVE://8

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

void renderMessage(Request *request) {
  printf("\n-----------------------------\nMessage:!\n");
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

    default:
      printf("%s\n", request->message);
      break;
  }
}