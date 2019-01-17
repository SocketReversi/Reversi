#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

#include "../../libs/request.h"
#include "../../libs/account.h"
#include "../../libs/file.h"
#include "../../libs/serverHandle.h"
#include "../../libs/reversi.h"

//create table game for client------------------------------------------------------------//
GSList *createTable(GSList *listTable, GSList *listUser, int id, char name[50], int point){

  table *var = malloc(sizeof(table));
  int rank = 1; //varible for rank of clients
  GSList *cur = listUser;
  while(cur != NULL){
    account *acc = cur->data;
    if(strcmp(name,acc->username) == 0){
      break;
    }
    rank += 1; 
    cur = cur->next;
  }

  var->master = id;    
  var->master_point = point;//add point master
  var->master_rank = rank;  //add rank master to table game
  strcpy(var->master_name , name);//add name master

  var->guest = EMPTY;   
  var->state = WAITING; 
  var->result = 1;
  var->current = BLACK;
  initialize(var->board);
  var->turn = var->master;

  //add table game to list of table game----------//
  listTable = g_slist_append(listTable, var);

  return listTable;

}

int joinTable(GSList *listTable, GSList *listUser, int id, char name[50], int point){

  GSList *var;
  var = listTable;

  int rank = 1; //varible for rank of guest
  GSList *cur = listUser;
  while(cur != NULL){
    account *acc = cur->data;
    if(strcmp(name,acc->username) == 0){
      break;
    }
    rank += 1; 
    cur = cur->next;
  }

  while(var != NULL){

    table *node = var->data;
    if(node->state == WAITING){ 
      int cachbiet = abs(node->master_rank - rank);
      printf("Cacsh biet : %d\n", cachbiet);
      if(cachbiet <= 10 ){ //If the rank is not more than 10 levels apart
          node->guest = id; 
          node->guest_point = point;
          node->guest_rank = rank; //add rank of guest
          strcpy(node->guest_name , name); //add name guest

          node->state = FULL; //update state of table game
          return 1; //join success 
      }

    }
    var = var->next;
  }
  return 0; //join fail
}

//Identify the player as master or guest---------//
int Player(GSList *listTable, int id){
  GSList *var = listTable;
  table *node;
  while(var != NULL){
    node = var->data;
    if(id == node->master)
      return MASTER;
    if(id == node->guest)
      return GUEST;
    var = var->next;
  }
  return 0;
}

//find user by id of client-----------------------------//
GSList *findWithID(GSList *listTable, int id){

  GSList *var;
  var = listTable;
  while(var != NULL){

    table *node = var->data;
    if(id == node->master || id == node->guest){
      return var; //find result - return this table game
    }
    var = var->next;
  }
  return NULL; //not found user
}


//leave table game--------------------------------//
GSList *leaveTable(GSList *listTable, int id){
  GSList *find = findWithID(listTable, id);
  if(find != NULL ){
    listTable = g_slist_remove_link(listTable, find); //delete table game in list table game of server
    return listTable;
  }
  return NULL;
}


//display list of table game ------------------------//
void printTable(GSList *list){
  GSList *var;
  var = list;
  printf("Table online (%d)\n",(int)g_slist_length(list));
  if(var == NULL){
    printf("\n[No table has been created]\n");
  }else{
    printf("%-10s|%-22s|%-22s\n"," STATE"," MASTER(rank)"," GUEST(rank)");
    printf(" -------------------------------------------------------\n");
    while(var != NULL){

      table *node = var->data;
      switch(node->state){
        case WAITING:
          printf(" %-9s","waiting");
          break;
        default:
          printf(" %-9s","FULL");
          break;
      }
      printf("| %-14s (%-4d)| %-14s (%-4d)\n",
        node->master_name,
        node->master_rank,
        node->guest_name,
        node->guest_rank
      );
      var = var->next;
    }
    printf(" -------------------------------------------------------\n\n");
  }
}

//display list user ----------------------------//
void printListUser(GSList *list){

  printf("Number user (%d)\n",(int)g_slist_length(list));
  GSList *var;
  var = list;
  int i = 1;
  printf("%-20s|%-20s|%-6s|%-10s|%-8s|%-10s\n"," USERNAME"," PASSWORD"," RANK"," POINT"," STATE"," STATUS");
  printf("\n ------------------------------------------------------------------------------\n");
  while(var != NULL){
    account *acc = var->data;
    char state[10],status[10];

    switch(acc->state){
      case 0:
        strcpy(state,"-");
        break;
      default:
        strcpy(state,"ON");
        break; 
    }

    switch(acc->status){
      case 0:
        strcpy(status,"block");
        break;
      default:
        strcpy(status,"ACTIVE");
        break;
    }
    
    printf("  %-18s|  %-18s| %-5d| %-9d|  %-6s| %-9s\n",
            acc->username, 
            acc->password, 
            i, // rank
            acc->point,
            state,
            status
          );
    printf(" ------------------------------------------------------------------------------\n");
    i++;
    var = var->next;
  }
  printf("\n");
}

//copy board game -----------------------------------//
void copyBoard(int board1[8][8], int board2[8][8]){

  for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      board1[i][j] = board2[i][j];
    }
  }
}

//find account with username & password--------------------//
account *find_User_Pass(GSList *list, Request *request){
  GSList *var;
  var = list;
  while(var != NULL){
    account *acc = var->data;
    if(strcmp(acc->username , request->username) == 0){
      if(strcmp(acc->password , request->password) == 0){
        return acc; //tai khoan dung mat khau
      }else{
        return NULL; //tai khoan sai mat khau
      }
    }
    var = var->next;
  }
  return NULL;
}

//find account with username ------------------------------//
account *find_User(GSList *list, Request *request){
  GSList *var;
  var = list;
  while(var != NULL){
    account *acc = var->data;
    if(strcmp(acc->username , request->username) == 0)
      return acc;
    var = var->next;
  }
  return NULL;
}

//find (in link list ) node contains the required information-------//
GSList *find(GSList *listUser, char name[50]){
  GSList *result;
  result = listUser;
  while(result != NULL){
    account *user = result->data;
    if(strcmp(user->username , name) == 0)
      return result; //return node you need
    result = result->next;
  }
  return NULL; // not found node
}

//register account----------------------------------------------------//
int Register(GSList *listUser, Request *request){
  account *user = createAccount(request->username,request->password, 0, 0 , 1);
  listUser = g_slist_append(listUser, user);
  listUser = g_slist_sort(listUser, (GCompareFunc)sortFunction);//sap xep lai cac tai khoan theo diem so
  updateData(listUser);
  return 1;
}

//find id playmate of user---------------------------------//
int findIDPlayMate(GSList *listTable, int idPlayer){
  int id, type;
  table *node = findWithID(listTable, idPlayer)->data;
  if(node == NULL)
    return -1;
  type = Player(listTable, idPlayer); //Determine the type of player in the table
  if(type != 0){
    if(type == MASTER)
      id = node->guest;
    else if(type == GUEST)
      id = node->master;
    else
      return -1;
    printf("ID : %d\n",id );
    return id; //return id of player in table game
  }
  return -1;
}

int findPlayMate(GSList *listTable, int id, int client[FD_SETSIZE]){ //find index of playmate--------//

  int index = 0;
  int type, id_player;
  table *node = findWithID(listTable, id)->data; //find table game have 2 player
  if(node == NULL)
    return -1;
  type = Player(listTable, id);
  if(type == MASTER)
    id_player = node->guest;
  else if(type == GUEST)
    id_player = node->master;
  else
    return -1;
  while(1){
    if(id_player == client[index] )
      return index;
    index++;
  }
  return -1;
}

//handing request of client---------------------------------------------------------------//
Request *handleRequest(int state, Request *request, GSList *listUser, char user[50])
{
  Request *sendRequest = malloc(sizeof(Request));
  GSList *ds = listUser;
  account *tk;
  int rankOfClient = 1;
  int pointClient;

  switch (request->opcode)
  {
    case LOGIN:
      if(state != UNKNOWN){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!Ex:");
        switch(state){
          case STATE1:
            strcat(sendRequest->message,"logout,create,join,rank");
            break;
          case STATE2:
            strcat(sendRequest->message,"play,leave");
            break;
          case STATE2_2:
            strcat(sendRequest->message,"move,leave");
            break;
          case STATE3:
            strcat(sendRequest->message,"leave");
            break;
        }
        break;
      }
      if(find_User(listUser, request) != NULL){
        if(find_User_Pass(listUser, request) != NULL){ //tim dung tai khoan ca username va password

          if(find_User_Pass(listUser, request)->status == 1){
            if(find_User_Pass(listUser, request)->state == 0){
              find_User_Pass(listUser, request)->state = 1; //trang thai da dang nhap thanh cong
              sendRequest->opcode = LOGIN_SUCCESS;
              strcpy(sendRequest->message,"Log in success ! Hello : ");
              strcpy(sendRequest->username,request->username);
            }
            else{
              sendRequest->opcode = LOGIN_FAIL;
              strcpy(sendRequest->message,"Error ! User is online!");
            }
          }
          else{
            sendRequest->opcode = LOGIN_FAIL;
            strcpy(sendRequest->message,"Error ! User is blocked!");
          }

        }else{
            sendRequest->opcode = PASS_WRONG; //password is wrong!
            strcpy(sendRequest->message,"Error ! Password invalid!");
        }

      }else{
        sendRequest->opcode = LOGIN_FAIL;
        strcpy(sendRequest->message,"Error! Username invalid!");
      }
      break;

    case REGISTER:
      if(state != UNKNOWN){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!Ex:");
        switch(state){
          case STATE1:
            strcat(sendRequest->message,"logout,create,join,rank");
            break;
          case STATE2:
            strcat(sendRequest->message,"play,leave");
            break;
          case STATE2_2:
            strcat(sendRequest->message,"move,leave");
            break;
          case STATE3:
            strcat(sendRequest->message,"leave");
            break;
        }
        break;
      }
      if(find_User(listUser, request) == NULL){
        Register(listUser,request);
        sendRequest->opcode = REGISTER_SUCCESS;
        strcpy(sendRequest->message,"Register success!");
      }else{
        sendRequest->opcode = REGISTER_FAIL;
        strcpy(sendRequest->message,"Error! Username has already existed");
      }
      break;

    case LOGOUT:
      if(state != STATE1){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!Ex:");
        switch(state){
          case UNKNOWN:
            strcat(sendRequest->message,"login,register");
            break;
          case STATE2:
            strcat(sendRequest->message,"play,leave");
            break;
          case STATE2_2:
            strcat(sendRequest->message,"move,leave");
            break;
          case STATE3:
            strcat(sendRequest->message,"leave");
            break;
        }
        break;
      }
      if(find_User(listUser, request) != NULL){
        if(find_User(listUser, request)->state == 1){
          if(strcmp(request->username, user) == 0){
            find_User(listUser, request)->state = 0;
            sendRequest->opcode = LOGOUT_SUCCESS;
            strcpy(sendRequest->message,"Log out success. Goodbye!");
          }else{
            sendRequest->opcode = LOGOUT_FAIL;
            strcpy(sendRequest->message,"Fail! Username wrong!");
          }
        }else{
          sendRequest->opcode = LOGOUT_FAIL;
          strcpy(sendRequest->message,"Fail! User is not logged in!");
        }

      }else{
        sendRequest->opcode = LOGOUT_FAIL;
        strcpy(sendRequest->message,"Error! Username does not exist"); 
      }
      break;

    case RANK:
      if(state != STATE1){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!Ex:");
        switch(state){
          case UNKNOWN:
            strcat(sendRequest->message,"login,register");
            break;
          case STATE2:
            strcat(sendRequest->message,"play,leave");
            break;
          case STATE2_2:
            strcat(sendRequest->message,"move,leave");
            break;
          case STATE3:
            strcat(sendRequest->message,"leave");
            break;
        }
        break;
      }else{

        while(ds != NULL){
          tk = ds->data;
          if(strcmp(tk->username, user) == 0){
            pointClient = tk->point;
            break;
          }
          rankOfClient ++;
          ds = ds->next;
        }

        sendRequest->rank = rankOfClient;
        sendRequest->point = pointClient;
        sendRequest->opcode = RANK;
        strcpy(sendRequest->message,"--- * Your rank * ---\n"); 
      }

      break;

    default:
      return NULL;
  }
  memset(request->message, '\0', 50);
  return sendRequest;
}

//Grouping for the client--------------------------------//
Request *groupClient(int state, Request *request, GSList *listTable, GSList *listUser, int client, char user[50]){

  Request *sendRequest = malloc(sizeof(Request));

  switch(request->opcode){

    case CREATE:
        if(state != STATE1){
          sendRequest->opcode = REQUEST_FAIL;
          strcpy(sendRequest->message,"Warning! Request invalid!Ex:");
          switch(state){
              case UNKNOWN:
                strcat(sendRequest->message,"login,register");
                break;
              case STATE2:
                strcat(sendRequest->message,"play,leave");
                break;
              case STATE2_2:
                strcat(sendRequest->message,"move,leave");
                break;
              case STATE3:
               strcat(sendRequest->message,"leave");
                break;
            }
          break;
        }
        sendRequest->opcode = CREATE_SUCCESS;
        strcpy(sendRequest->message,"Create table game success!");
      break;

    case JOIN:

      if(state != STATE1){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!Ex:");
        switch(state){
            case UNKNOWN:
              strcat(sendRequest->message,"login,register");
              break;
            case STATE2:
              strcat(sendRequest->message,"play,leave");
              break;
            case STATE2_2:
              strcat(sendRequest->message,"move,leave");
              break;
            case STATE3:
              strcat(sendRequest->message,"leave");
              break;
          }
        break;
      }
      else{

        if(joinTable(listTable, listUser, client, user, ((account *)find(listUser, user)->data)->point ) == 0){
          sendRequest->opcode = JOIN_FAIL;
          strcpy(sendRequest->message,"No matching table found!"); //No players found matching the client
        }else{
          sendRequest->opcode = JOIN_SUCCESS;
          strcpy(sendRequest->message,"Success! The player has joined");
        }
      }


      break;

    case LEAVE:
      if(state != STATE2 && state != STATE2_2 && state != STATE3){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!Ex:");
        switch(state){
          case UNKNOWN:
            strcat(sendRequest->message,"login,register");
            break;
          case STATE1:
            strcat(sendRequest->message,"logout,create,join,rank");
            break;
        }
        break;
      }

      if(findWithID(listTable, client) != NULL){ // if Find players in the table
        sendRequest->opcode = LEAVE_SUCCESS;
        strcpy(sendRequest->message,"The player has left. Canceled the table game!");
      }else{
        sendRequest->opcode = LEAVE_FAIL;
        strcpy(sendRequest->message,"Leave fail!");
      }
      break;

    default:
      return NULL;
  }
  memset(request->message, '\0', 50);
  return sendRequest;
}

//handing play game of client-----------------------------------------------------//
Request *playGame(int state, Request *request, GSList *listTable, int client){

  Request *sendRequest = malloc(sizeof(Request));
  value message; //varible for save state table game
  table *node;
  GSList *var;

  switch(request->opcode){

    case PLAY:

      if(state != STATE2){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!Ex:");
        switch(state){
          case STATE1:
            strcat(sendRequest->message,"logout,create,join,rank");
            break;
          case UNKNOWN:
            strcat(sendRequest->message,"login,register");
            break;
          case STATE2_2:
            strcat(sendRequest->message,"move,leave");
            break;
          case STATE3:
            strcat(sendRequest->message,"leave");
            break;
        }
        break;
      }

      if(findWithID(listTable, client) != NULL){
        sendRequest->opcode = PLAY_SUCCESS;
        initialize(sendRequest->board);
        strcpy(sendRequest->message,"OK! Let 's play!");
      }else{
        sendRequest->opcode = PLAY_FAIL;
        strcpy(sendRequest->message,"Can't play! You have not joined any table yet");
      }

      break;

    case MOVE:
      if(state != STATE2_2 && state != STATE3 ){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!Ex:");
        switch(state){
          case STATE1:
            strcat(sendRequest->message,"logout,create,join,rank");
            break;
          case STATE2:
            strcat(sendRequest->message,"play,leave");
            break;
          case UNKNOWN:
            strcat(sendRequest->message,"login,register");
            break;
        }
        break;
      }
      var = findWithID(listTable, client);
      node = var->data;

      if(var != NULL){ 
        if(node->result > -1){ // if this table game is not over

          message = reverse(request->doc, 
                            request->ngang, 
                            node->board,
                            node->current
                            );

          //copy board game for client 
          copyBoard(sendRequest->board, node->board);

          //display board game--------------------------//
          display(sendRequest->board);

          node->result = message.state;
          node->current = message.color;

          if(node->result == -1){// if table game is over
            sendRequest->opcode = END_GAME;
            if(winner(sendRequest->board) == BLACK)
              strcpy(sendRequest->message,"WINNER : ○  ! The game end!\n");
            else if(winner(sendRequest->board) == WHITE)
              strcpy(sendRequest->message,"WINNER : ●  ! The game end!\n");
            else if(winner(sendRequest->board) == NONE)
              strcpy(sendRequest->message,"GAME DRAW! The game end!\n");
          }

          else if(node->result == 0 ){
            sendRequest->opcode = MOVE_FAIL;
            strcpy(sendRequest->message,"Move invalid!");
          }else{
            sendRequest->opcode = MOVE_SUCCESS;
            strcpy(sendRequest->message,"Your turn...");
          }
        }
      }
      else{
        sendRequest->opcode = MOVE_FAIL;
        strcpy(sendRequest->message,"You have not joined any table yet!");
      }
      break;

    default:
      strcpy(sendRequest->message,"Warning! Request invalid!");
  }
  memset(request->message, '\0', 50);
  return sendRequest;
}
