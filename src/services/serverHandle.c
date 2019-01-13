#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/request.h"
#include "../../libs/account.h"
#include "../../libs/file.h"
#include "../../libs/serverHandle.h"
#include "../../libs/reversi.h"

//dung link list----------------------//
GSList *createTable(GSList *listTable, int id){

  table *var = malloc(sizeof(table));

  var->master = id;
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

int joinTable(GSList *listTable, int id){

  GSList *var;
  var = listTable;
  while(var != NULL){

    table *node = var->data;
    if(node->state == WAITING){ //neu ban choi thieu nguoi
      printf("Tien hanh them nguoi choi\n");
      node->guest = id; //them khach vao ban choi
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
  printf("So ban choi online : (%d)\n",(int)g_slist_length(list));
  if(var == NULL){
    printf("Chua co ban choi nao duoc tao!\n");
  }
  while(var != NULL){
    table *node = var->data;
    printf("State:%d\nMaster:%d\nGuest:%d\n\n",node->state,node->master,node->guest);
    var = var->next;
  }
}

//hien thi danh sach tai khoan trong he thong-----------?
void printListUser(GSList *list){

  printf("Number account : (%d)\n",(int)g_slist_length(list));
  GSList *var;
  var = list;
  printf("%-20s|%-20s|%-6s|%-6s|%-6s\n",
          "Username",
          "Password",
          "Point",
          "State",
          "Status"
        );
  while(var != NULL){
    account *acc = var->data;
    printf("%-20s|%-20s|   %-3d|   %-3d|   %-3d\n",
            acc->username, 
            acc->password, 
            acc->point, 
            acc->state, 
            acc->status
          );

    var = var->next;
  }
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
        return acc;
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

int Register(GSList *listUser, Request *request){
  account *user = createAccount(request->username,request->password, 0, 0 , 1);
  listUser = g_slist_append(listUser, user);
  updateData(listUser);
  return 1;
}

Request *handleRequest(Request *request, GSList *listUser)
{
  Request *sendRequest = malloc(sizeof(Request));

  switch (request->opcode)
  {
    case LOGIN:
      if(find_User_Pass(listUser, request) != NULL){
        if(find_User_Pass(listUser, request)->state == 0){
          find_User_Pass(listUser, request)->state = 1; //trang thai da dang nhap thanh cong
          sendRequest->opcode = LOGIN_SUCCESS;
          strcpy(sendRequest->message,"Succerss! Dang nhap thanh cong! Xin chao");
        }
        else{
          sendRequest->opcode = LOGIN_FAIL;
          strcpy(sendRequest->message,"Fail! Tai khoan dang truc tuyen!");
        }
        
      }else{
        sendRequest->opcode = LOGIN_FAIL;
        strcpy(sendRequest->message,"Fail! Tai khoan hoac mat khau khong dung!");
      }
      printListUser(listUser);
      break;

    case LOGOUT:
      if(find_User(listUser, request) != NULL){
        if(find_User(listUser, request)->state == 1){
          find_User(listUser, request)->state = 0;
          sendRequest->opcode = LOGOUT_SUCCESS;
          strcpy(sendRequest->message,"Success! Tai khoan da dang xuat! Goodbye ");
        }else{
          sendRequest->opcode = LOGOUT_FAIL;
          strcpy(sendRequest->message,"Fail!Tai khoan chua dang nhap!");
        }

      }else{
        sendRequest->opcode = LOGOUT_FAIL;
        strcpy(sendRequest->message,"Fail!Dang xuat that bai!"); 
      }
      printListUser(listUser);
      break;

    case REGISTER:
      if(find_User(listUser, request) == NULL){
        Register(listUser,request);
        sendRequest->opcode = REGISTER_SUCCESS;
        strcpy(sendRequest->message,"Success! Dang ki thanh cong!");
      }else{
        sendRequest->opcode = REGISTER_FAIL;
        strcpy(sendRequest->message,"Fail! Dang ki that bai!");
      }
      printListUser(listUser);
      break;

    default:
      return NULL;
  }
  memset(request->message, '\0', 50);
  return sendRequest;
}


Request *groupClient(Request *request, GSList *listTable, int client){

  Request *sendRequest = malloc(sizeof(Request));

  switch(request->opcode){

    case CREATE:
        sendRequest->opcode = CREATE_SUCCESS;
        strcpy(sendRequest->message,"Success! Tao phong thanh cong!");
      break;

    case JOIN:
      if(joinTable(listTable, client) == 0){
        sendRequest->opcode = JOIN_FAIL;
        strcpy(sendRequest->message,"Fail! Khong tim thay ban choi!");
      }else{
        sendRequest->opcode = JOIN_SUCCESS;
        strcpy(sendRequest->message,"Success! Da co nguoi tham gia ban choi!");
      }
      //printTable(table);
      break;

    case LEAVE:
      if(findWithID(listTable, client) != NULL){ //tim thay nguoi choi trong ban
        sendRequest->opcode = LEAVE_SUCCESS;
        strcpy(sendRequest->message,"Nguoi choi da roi ban!Ban choi da huy!");
      }else{
        sendRequest->opcode = LEAVE_FAIL;
        strcpy(sendRequest->message,"Roi ban that bai!");
      }
      break;

    case CHECK:
      sendRequest->opcode = CHECK;
      if(Player(listTable, client) == MASTER)
        strcpy(sendRequest->message,"Ban la Master\n");
      if(Player(listTable, client) == GUEST)
        strcpy(sendRequest->message,"Ban la Guest\n");
      break;

    case CHAT:
      sendRequest->opcode = CHAT;
      strcpy(sendRequest->message,request->message);
      break;   

    default:
      return NULL;
  }
  memset(request->message, '\0', 50);
  return sendRequest;
}

Request *playGame(Request *request, GSList *listTable, int client){

  Request *sendRequest = malloc(sizeof(Request));
  value message; //tao bien luu trang thai ban co
  table *node;
  GSList *var;

  switch(request->opcode){

    case PLAY:

      if(findWithID(listTable, client) != NULL){
        sendRequest->opcode = PLAY_SUCCESS;
        initialize(sendRequest->board);
        strcpy(sendRequest->message,"OK! Bat dau choi");
      }else{
        sendRequest->opcode = PLAY_FAIL;
        strcpy(sendRequest->message,"Khong the choi! Chua tham gia ban choi nao!");
      }

      break;

    case MOVE:

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

          display(sendRequest->board);

          node->result = message.state;
          node->current = message.color;

          if(node->result == -1){//game the end
            sendRequest->opcode = END_GAME;
            if(winner(sendRequest->board) == BLACK)
              strcpy(sendRequest->message,"Winner : x ! Tran dau ket thuc!\n");
            else if(winner(sendRequest->board) == WHITE)
              strcpy(sendRequest->message,"Winner : o ! Tran dau ket thuc!\n");
            else if(winner(sendRequest->board) == NONE)
              strcpy(sendRequest->message,"Game draw! Tran dau ket thuc!\n");
          }

          else if(node->result == 0 ){
            sendRequest->opcode = MOVE_FAIL;
            strcpy(sendRequest->message,"Nuoc co FAIL!");
          }else{
            sendRequest->opcode = MOVE_SUCCESS;
            strcpy(sendRequest->message,"Luot cua ban!");
          }
        }
      }
      else{
        sendRequest->opcode = MOVE_FAIL;
        strcpy(sendRequest->message,"Chua tham gia ban choi nao!");
      }
      break;

    default:
      strcpy(sendRequest->message,"Thao tac khong hop le");
  }
  memset(request->message, '\0', 50);
  return sendRequest;
}
