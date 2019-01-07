#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/request.h"
#include "../../libs/account.h"
#include "../../libs/file.h"
#include "../../libs/serverHandle.h"

void createTableList(tableGame table[MAX_TABLE]){
  for(int i=0;i<MAX_TABLE;i++){
    table[i].state = EMPTY;
    table[i].master = EMPTY;
    table[i].guest = EMPTY;
    table[i].id = i;    
  }
}

int createTable(int IDmaster,tableGame table[MAX_TABLE]){
  for(int i=0;i<MAX_TABLE;i++){
    if(table[i].state == EMPTY){
      table[i].master = IDmaster;
      table[i].state  = WAITING;
      return 1; //tao thanh cong 1 ban choi moi
    }
  }
  return 0; //khong tao duoc ban choi
}

int joinTable(int IDguest, tableGame table[MAX_TABLE]){
  for(int i=0 ; i<MAX_TABLE ; i++){
    if(table[i].state == WAITING){
      table[i].guest = IDguest;
      table[i].state = FULL;
      return 1; //tham gia ban choi thanh cong
    }
  }
  return 0; //khong tham gia duoc ban choi nao
}

int findIDgamer(int IDgamer,tableGame table[MAX_TABLE]){
  for(int i=0; i<MAX_TABLE ; i++){
    if(IDgamer == table[i].master)
      return table[i].id;
    else if(IDgamer == table[i].guest){
      return (MAX_TABLE + table[i].id);
    }
  }
  return EMPTY;
}

int findID(int id, tableGame table[MAX_TABLE]){
  for(int i=0; i<MAX_TABLE ; i++){
    if(id == table[i].master || id == table[i].guest)
      return table[i].id;
  }
  return EMPTY;
}

int leaveTable(int IDgamer, tableGame table[MAX_TABLE]){
  int find = findID(IDgamer,table);
  if(find != EMPTY ){
    table[find].master = EMPTY;
    table[find].guest  = EMPTY;
    table[find].state  = EMPTY;
    return find;
  }
  return EMPTY;
}

void printTable(tableGame table[MAX_TABLE]){
  for(int i=0 ; i<MAX_TABLE ; i++){
    printf("IDTable:%d\nState:%d\nMaster:%d\nGuest:%d\n\n",table[i].id,table[i].state,table[i].master,table[i].guest);
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

Request *handleRequest(Request *request, GSList *listUser)
{
  Request *sendRequest = malloc(sizeof(Request));
  memset(request->message, '\0', 50);

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
        account *user = createAccount(request->username,request->password, 0, 0 , 1);
        listUser = g_slist_append(listUser, user);
        updateData(listUser);
        strcpy(sendRequest->message,"Success! Dang ki thanh cong!");
      }else{
        sendRequest->opcode = REGISTER_FAIL;
        strcpy(sendRequest->message,"Fail! Dang ki that bai 'Ten tai khoan da ton tai!'");
      }
      //updateData(listUser);
      printListUser(listUser);
      break;

    default:
      return NULL;
  }

  return sendRequest;
}


Request *groupClient(Request *request, tableGame table[MAX_TABLE], int client){

  Request *sendRequest = malloc(sizeof(Request));
  memset(request->message, '\0', 50);

  switch(request->opcode){

    case CREATE:
      if(createTable(client, table) == 0){
        sendRequest->opcode = CREATE_FAIL;
        strcpy(sendRequest->message,"Fail!Tao phong that bai!");
      }else{
        sendRequest->opcode = CREATE_SUCCESS;
        strcpy(sendRequest->message,"Success! Tao phong thanh cong");
      }
      break;

    case JOIN:
      break;

    case LEAVE:
      break;

    default:
      return NULL;
  }

  return sendRequest;
}