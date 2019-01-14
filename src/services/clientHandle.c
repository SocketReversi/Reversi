#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/request.h"
#include "../../libs/reversi.h"
#include "../../libs/clientHandle.h"

void printHelp(){
  printf("        [help]        \n");
  printf("|%-10s|%-10s|\n","  Opcode","  Mean");
  printf(" --------------------\n");
  printf("|   %-7d|%-10s|\n",  1," LOG IN");
  printf("|   %-7d|%-10s|\n",  2," REGISTER");
  printf("|   %-7d|%-10s|\n",  3," LOG OUT");
  printf("|   %-7d|%-10s|\n",  4," CREATE");
  printf("|   %-7d|%-10s|\n",  5," JOIN");
  printf("|   %-7d|%-10s|\n",  6," LEAVE");
  printf("|   %-7d|%-10s|\n",  7," PLAY");
  printf("|   %-7d|%-10s|\n",  8," MOVE");
  printf(" --------------------\n");
}

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

    if(strcmp(input,"help") == 0){
      printHelp();
    }else if(select <LOGIN || select > MOVE){
      printf("Request invalid! Input [help] for instructions!\n");
    }

  } while(select < LOGIN || select > MOVE);

  switch(select){
    char choice[10];
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
      printf("CREATE\n");
      printf("Waitting...\n");
      break;

    case JOIN: //5
      printf("JOIN\n");
      printf("Waitting...\n");
      break;

    case LEAVE://6
      printf("LEAVE\n");
      printf("Do you really want to leave the table game?\n");
      printf(" [yes] -> Agree!        [other] -> Cancel! \n");
      printf("Choice : ");scanf("%s",choice);
      if(strcmp(choice,"yes") == 0){
        select = LEAVE;
      }else{
        select = RESET;
      }
      break;

    case PLAY://7
      printf("PLAY\n");
      break;

    case MOVE://8

      printf("MOVE\nVertical   : ");
      scanf("%d",&request->doc);
      printf("Horizontal : ");
      scanf("%d",&request->ngang);
      while(request->doc < 0 || request->doc >7 || request->ngang < 0 || request->ngang >7){
        
        printf("Input invalid! Again!\n");
        printf("MOVE\nVertical   : ");
        scanf("%d",&request->doc);
        printf("Horizontal : ");
        scanf("%d",&request->ngang);
      }
      break;

  }
  // Update request store send to server
  request->opcode = select;

  return request;
}

int renderMessage(Request *request) {
  printf("\n[Reply from server]-> ");
  switch(request->opcode){

    case LOGIN_SUCCESS:
      printf("%s %s\n", request->message, request->username);
      break;

    case LOGOUT_SUCCESS:
      printf("%s %s\n", request->message, request->username);
      break;

    case PLAY_SUCCESS:
      display(request->board);
      if(request->turn == 0){
        printf("Waitting ...\n");
        return 1;
      }

        
      break;

    case MOVE_SUCCESS:
      display(request->board);
      if(request->turn == 0){
        printf("Waitting ...\n");
        return 1;
      }
      printf("%s\n", request->message);
      break;

    case MOVE_FAIL:
      display(request->board);
      printf("%s\n", request->message);
      break;

    case CREATE_SUCCESS:
      printf("%s\n", request->message);
      printf("Wait for other player ...\n");
      return 1;

    default:
      printf("%s\n", request->message);
      break;
  }

  return 0; //khong co yeu cau doi them
}