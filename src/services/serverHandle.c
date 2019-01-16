#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

#include "../../libs/account.h"
#include "../../libs/request.h"
#include "../../libs/file.h"
#include "../../libs/serverHandle.h"
#include "../../libs/reversi.h"

//dung link list----------------------//
GSList *createTable(GSList *listTable, int id, char name[50]){

  table *var = malloc(sizeof(table));

  var->master = id;
  strcpy(var->master_name , name); //them ten nguoi choi master
  var->guest = EMPTY;
  var->state = WAITING;
  var->result = 1;
  var->current = BLACK;
  initialize(var->board);
  var->turn = var->master;

  //them ban choi vao danh sach chay---//
  listTable = g_slist_append(listTable, var);

  return listTable;

}

int joinTable(GSList *listTable, int id, char name[50]){

  GSList *var;
  var = listTable;
  while(var != NULL){

    table *node = var->data;
    if(node->state == WAITING){ //neu ban choi thieu nguoi
      node->guest = id; //them khach vao ban choi
      strcpy(node->guest_name , name); //them ten nguoi choi guest
      node->state = FULL; //cap nhat trang thai ban choi FULL
      return 1; //join thanh cong
    }
    var = var->next;
  }
  return 0; //khong co ban choi nao trong
}

//xac dinh loai nguoi choi la Master hay Guest--//
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

//tim theo ID dung link list
GSList *findWithID(GSList *listTable, int id){

  GSList *var;
  var = listTable;
  while(var != NULL){

    table *node = var->data;
    if(id == node->master || id == node->guest){ //neu tim thay nguoi choi
      return var; //tim thay nguoi choi - tra ve ban choi do
    }
    var = var->next;
  }
  return NULL; //khong tim thay nguoi choi
}

GSList *leaveTable(GSList *listTable, int id){
  GSList *find = findWithID(listTable, id);
  if(find != NULL ){
    listTable = g_slist_remove_link(listTable, find); //xoa ban choi
    return listTable; //xoa ban choi
  }
  return NULL;
}

void printTable(GSList *list){
  GSList *var;
  var = list;
  printf("Table online (%d)\n",(int)g_slist_length(list));
  if(var == NULL){
    printf("\n[No table has been created]\n");
  }else{
    printf("%-10s|%-15s|%-15s\n"," STATE"," MASTER"," GUEST");
    printf(" -----------------------------------------\n");
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
      printf("| %-14s| %-14s\n",
        node->master_name,
        node->guest_name
      );
      var = var->next;
    }
    printf(" -----------------------------------------\n\n");
  }
}

//hien thi danh sach tai khoan trong he thong?-----------//
void printListUser(GSList *list){

  printf("Number user (%d)\n",(int)g_slist_length(list));
  GSList *var;
  var = list;
  printf("%-20s|%-20s|%-10s|%-8s|%-10s\n"," USERNAME"," PASSWORD"," POINT"," STATE"," STATUS");
  printf("\n ----------------------------------------------------------------------\n");
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
    
    printf("  %-18s|  %-18s| %-9d|  %-6s| %-9s\n",
            acc->username, 
            acc->password, 
            acc->point,
            state,
            status
          );
    printf(" ----------------------------------------------------------------------\n");
    var = var->next;
  }
  printf("\n");
}

void copyBoard(int board1[8][8], int board2[8][8]){

  for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      board1[i][j] = board2[i][j];
    }
  }
}

//tim tai khoan theo ten va mat khau--------------------//
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

//tim tai khoan theo ten------------------------------//
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

//tim node trong GSList chua thong tin yeu cau-------//
GSList *find(GSList *listUser, char name[50]){
  GSList *result;
  result = listUser;
  while(result != NULL){
    account *user = result->data;
    if(strcmp(user->username , name) == 0)
      return result;
    result = result->next;
  }
  return NULL; // neu khong tim thay 
}

int Register(GSList *listUser, Request *request){
  account *user = createAccount(request->username,request->password, 0, 0 , 1);
  listUser = g_slist_append(listUser, user);
  updateData(listUser);
  return 1;
}

int findIDPlayMate(GSList *listTable, int idPlayer){
  int id, type;
  table *node = findWithID(listTable, idPlayer)->data;
  if(node == NULL)
    return -1;
  type = Player(listTable, idPlayer); //xac dinh kieu nguoi choi trong ban
  if(type != 0){
    if(type == MASTER)
      id = node->guest;
    else if(type == GUEST)
      id = node->master;
    else
      return -1;
    printf("ID : %d\n",id );
    return id; //tra ve id nguoi cung choi trong ban
  }
  return -1;
}

int findPlayMate(GSList *listTable, int id, int client[FD_SETSIZE]){ //tim index cua ban cung ban choi--------//

  int index = 0;
  int type, id_player;
  table *node = findWithID(listTable, id)->data; //tim ban choi chua hai nguoi
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
      return index; //vi tri index trong mang user,client,stateClient,count - ko phai id
    index++;
  }
  return -1;
}

Request *handleRequest(int state, Request *request, GSList *listUser, char user[50])
{
  Request *sendRequest = malloc(sizeof(Request));

  switch (request->opcode)
  {
    case LOGIN:
      if(state != UNKNOWN){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!");
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
      //printListUser(listUser);
      break;

    case REGISTER:
      if(state != UNKNOWN){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!");
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
      //printListUser(listUser);
      break;

    case LOGOUT:
      if(state != STATE1){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!");
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
      //printListUser(listUser);
      break;

    case RANK:
      if(state != STATE1){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!");
        break;
      }else{
        //xu ly yeu cau muon xem Rank cua client
        /* code here*/
        GSList *rank = g_slist_sort(listUser, (GCompareFunc)sortFunction);
        //tra ve opcode va thong diep cho client

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


Request *groupClient(int state, Request *request, GSList *listTable, int client, char user[50]){

  Request *sendRequest = malloc(sizeof(Request));

  switch(request->opcode){

    case CREATE:
        if(state != STATE1){
          sendRequest->opcode = REQUEST_FAIL;
          strcpy(sendRequest->message,"Warning! Request invalid!");
          break;
        }
        sendRequest->opcode = CREATE_SUCCESS;
        strcpy(sendRequest->message,"Create table game success!");
      break;

    case JOIN:

      if(state != STATE1){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!");
        break;
      }

      if(joinTable(listTable, client, user) == 0){
        sendRequest->opcode = JOIN_FAIL;
        strcpy(sendRequest->message,"Not found table game online!");
      }else{
        sendRequest->opcode = JOIN_SUCCESS;
        strcpy(sendRequest->message,"Success! The player has joined");
      }

      break;

    case LEAVE:
      if(state != STATE2 && state != STATE2_2 && state != STATE3){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!");
        break;
      }

      if(findWithID(listTable, client) != NULL){ //tim thay nguoi choi trong ban
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

Request *playGame(int state, Request *request, GSList *listTable, int client){

  Request *sendRequest = malloc(sizeof(Request));
  value message; //tao bien luu trang thai ban co
  table *node;
  GSList *var;

  switch(request->opcode){

    case PLAY:

      if(state != STATE2){
        sendRequest->opcode = REQUEST_FAIL;
        strcpy(sendRequest->message,"Warning! Request invalid!");
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
        strcpy(sendRequest->message,"Warning! Request invalid!");
        break;
      }
      var = findWithID(listTable, client);
      node = var->data;

      if(var != NULL){ //neu tim thay nguoi trong o trong ban choi
        if(node->result > -1){ // neu tro choi chua ket thuc

          message = reverse(request->doc, 
                            request->ngang, 
                            node->board,
                            node->current
                            );

          //copy co pho de gui cho client 
          copyBoard(sendRequest->board, node->board);

          //HIEN THI BAN CO --------------------------//
          display(sendRequest->board);

          node->result = message.state;
          node->current = message.color;

          if(node->result == -1){//game the end
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
