#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>

#include <unistd.h>

#include "../libs/account.h"
#include "../libs/request.h"
#include "../libs/serverHandle.h"
#include "../libs/valid.h"
#include "../libs/file.h"
#include "../libs/reversi.h"

int main(int argc, char *argv[]){
  paramsServerValid(argc);
  // Set open port
  int PORT = atoi(argv[1]);

  //Init table for game
  GSList *listTable = NULL;

  //Init user data
  GSList *listUser = importUserFromFileToList();
  printListUser(listUser);

  int i, maxi, maxfd, listenfd, connfd, sockfd;
  int nready, client[FD_SETSIZE]; //so may khach toi da
  int stateClient[FD_SETSIZE]; //trang thai tuong ung voi moi tai khoan Client
  char user[FD_SETSIZE][50]; //so tai khoan tuong ung voi so may khach
  int count[FD_SETSIZE]; //so bien dem cho cac tai khoan may khach

  ssize_t ret;
  fd_set readfds, allset;
  socklen_t clilen;
  struct sockaddr_in cliaddr, servaddr;
  Request *sendBuff;
  Request *rcvBuff = malloc(sizeof(Request));

  //Step 1: Construct a TCP socket to listen connection request
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  { /* calls socket() */
    perror("\nError: ");
    return 0;
  }

  //Step 2: Bind address to socket
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
  { /* calls bind() */
    perror("\nError: ");
    return 0;
  }

  //Step 3: Listen request from client
  if (listen(listenfd, BACKLOG) == -1)
  { /* calls listen() */
    perror("\nError: ");
    return 0;
  }

  maxfd = listenfd; /* initialize */
  maxi = -1;        /* index into client[] array */
  for (i = 0; i < FD_SETSIZE; i++)
  {
    client[i] = -1; /* -1 indicates available entry */
    stateClient[i] = UNKNOWN;
  }
  FD_ZERO(&allset);
  FD_SET(listenfd, &allset);
  //Step 4: Communicate with clients
  while (1)
  {
    readfds = allset; /* structure assignment */
    nready = select(maxfd + 1, &readfds, NULL, NULL, NULL);
    if (nready < 0)
    {
      perror("\nError: ");
      return 0;
    }

    if (FD_ISSET(listenfd, &readfds))
    { /* new client connection */
      clilen = sizeof(cliaddr);
      if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
        perror("\nError: ");
      else
      {
        printf("You got a connection from %s\n", inet_ntoa(cliaddr.sin_addr)); /* prints client's IP */
        for (i = 0; i < FD_SETSIZE; i++)
          if (client[i] < 0)
          {
            client[i] = connfd; /* save descriptor */
            break;
          }
        if (i == FD_SETSIZE)
        {
          printf("\nToo many clients");
          close(connfd);
        }

        FD_SET(connfd, &allset); /* add new descriptor to set */
        if (connfd > maxfd)
          maxfd = connfd; /* for select */
        if (i > maxi)
          maxi = i; /* max index in client[] array */

        if (--nready <= 0)
          continue; /* no more readable descriptors */
      }
    }

    for (i = 0; i <= maxi; i++)
    { /* check all clients for data */
      if ((sockfd = client[i]) < 0)
        continue;
      if (FD_ISSET(sockfd, &readfds))
      {
        ret = receiveData(sockfd, rcvBuff, sizeof(Request), 0);
        if (ret <= 0)
        {
          FD_CLR(sockfd, &allset);
          close(sockfd);
          client[i] = -1;
        }
        else
        {
          sendBuff = groupClient(stateClient[i], rcvBuff, listTable, sockfd, user[i]);
          
          if(sendBuff == NULL){
            sendBuff = handleRequest(stateClient[i], rcvBuff,listUser,user[i]);
          }
          if(sendBuff == NULL){
            sendBuff = playGame(stateClient[i], rcvBuff, listTable, sockfd);
          }

          //setting State for client---------------//
            if( sendBuff->opcode == LOGIN_FAIL||
                sendBuff->opcode == PASS_WRONG||
                sendBuff->opcode == REGISTER_SUCCESS||
                sendBuff->opcode == REGISTER_FAIL||
                sendBuff->opcode == LOGOUT_SUCCESS
              ){
              stateClient[i] = UNKNOWN;

            }else if(
                sendBuff->opcode == LOGIN_SUCCESS||
                sendBuff->opcode == LOGOUT_FAIL||
                sendBuff->opcode == LEAVE_SUCCESS||
                sendBuff->opcode == CREATE_FAIL||
                sendBuff->opcode == JOIN_FAIL||
                sendBuff->opcode == END_GAME
              ){
              stateClient[i] = STATE1;

            }else if(
                sendBuff->opcode == CREATE_SUCCESS||
                sendBuff->opcode == JOIN_SUCCESS||
                sendBuff->opcode == LEAVE_FAIL
              ){
              stateClient[i] = STATE2;

            }else if(
                sendBuff->opcode == PLAY_SUCCESS||
                sendBuff->opcode == MOVE_SUCCESS||
                sendBuff->opcode == MOVE_FAIL
              ){
              stateClient[i] = STATE3;

            }else if(
              sendBuff->opcode == MOVE_SUCCESS
              ){
              stateClient[i] = STATE4;

            }
          //end setting state for client-----------//

          //SEND DATA TO CLIENT--------------------------//

          if(   sendBuff->opcode == LOGIN_SUCCESS||
                sendBuff->opcode == LOGIN_FAIL||
                sendBuff->opcode == PASS_WRONG||
                sendBuff->opcode == REGISTER_SUCCESS||
                sendBuff->opcode == REGISTER_FAIL||

                sendBuff->opcode == LOGOUT_SUCCESS||
                sendBuff->opcode == LOGOUT_FAIL||
                sendBuff->opcode == JOIN_FAIL||
                sendBuff->opcode == LEAVE_FAIL||
                
                sendBuff->opcode == PLAY_FAIL||
                sendBuff->opcode == MOVE_FAIL||
                // sendBuff->opcode == CHECK||
                sendBuff->opcode == REQUEST_FAIL){

            if(sendBuff->opcode == LOGIN_SUCCESS){
              count[i] = 0;
              strcpy(user[i], sendBuff->username); //lay thong tin ve user dang online tuong ung voi may khach
            }
            else if(sendBuff->opcode == PASS_WRONG){//neu sai mat khau
              count[i] += 1; 
              if(count[i] == 3){ //neu tai khoan nhap sai mat khau qua 3 lan 
                account *acc = find(listUser, rcvBuff->username)->data;
                acc->status = 0; //block tai khoan vi sai mat khau qua 3 lan
                strcpy(sendBuff->message, "Password invalid 3 time! User is blocked!");
                updateData(listUser);
              }
            }
            ret = sendData(sockfd, sendBuff, sizeof(Request), 0);

          }else if(sendBuff->opcode == CREATE_SUCCESS){
            listTable = createTable(listTable, sockfd, user[i]);

            ret = sendData(sockfd, sendBuff, sizeof(Request), 0);
          }

          else if(sendBuff->opcode == LEAVE_SUCCESS || sendBuff->opcode == END_GAME){
            if(sendBuff->opcode == END_GAME){
              int result = winner(sendBuff->board); // ket qua tran co

              GSList *man1 = find(listUser, user[i]); //node chua thong tin nguoi 1
              GSList *man2 = find(listUser, user[findPlayMate(listTable, sockfd, client)]); //node chua thong tin nguoi 2
              account *acc1 = man1->data;
              account *acc2 = man2->data;

              int type = Player(listTable, sockfd);  //xac dinh vai tro nguoi choi 1
          
              if(result == BLACK){  //master thang
                if(type = MASTER){
                  acc1->point += POINT;
                  acc2->point -= POINT;

                }else if(type = GUEST){
                  acc1->point -= POINT;
                  acc2->point += POINT;
                }
              }else if(result = WHITE){ //guest thang
                if(type = GUEST){
                  acc1->point += POINT;
                  acc2->point -= POINT;

                }else if(type = MASTER){
                  acc1->point -= POINT;
                  acc2->point += POINT;
                }
              }
            }

            else if(sendBuff->opcode == LEAVE_SUCCESS){
              GSList *man1 = find(listUser, user[i]); //node chua thong tin nguoi 1
              GSList *man2 = find(listUser, user[findPlayMate(listTable, sockfd, client)]); //node chua thong tin nguoi 2
              account *acc1 = man1->data;
              account *acc2 = man2->data;

              acc1->point -= POINT;
              acc2->point += POINT;

              //nguoi nao tu y thoat ra se bi tru 100 diem, nguoi con lai duoc cong 100 diem
            }

            table *node = findWithID(listTable, sockfd)->data;
            sendData(node->master, sendBuff, sizeof(Request), 0);
            sendData(node->guest, sendBuff, sizeof(Request), 0);
            int index = findPlayMate(listTable, sockfd, client);
            listTable = leaveTable(listTable, sockfd);
            updateData(listUser);
            stateClient[index] = STATE1;
          }

          else if(sendBuff->opcode == MOVE_SUCCESS || sendBuff->opcode == PLAY_SUCCESS){
            table *node = findWithID(listTable, sockfd)->data;

            if(node->current == BLACK){
              sendBuff->turn = 1;
              sendData(node->master, sendBuff, sizeof(Request), 0);
              sendBuff->turn = 0;
              sendData(node->guest, sendBuff, sizeof(Request), 0);

            }else if(node->current = WHITE){
              sendBuff->turn = 1;
              sendData(node->guest, sendBuff, sizeof(Request), 0);
              sendBuff->turn = 0;
              sendData(node->master, sendBuff, sizeof(Request), 0);
            }

          }
          else {
            
            int type = Player(listTable, sockfd); 

            table *node = (table *)findWithID(listTable, sockfd)->data;

            if(type == MASTER){
                sendData(node->guest, sendBuff, sizeof(Request), 0);
            }
            else if(type == GUEST){
              sendData(node->master, sendBuff, sizeof(Request), 0);
            }else{
              ret = sendData(sockfd, sendBuff, sizeof(Request), 0);
            }
          }
          printListUser(listUser); //hien thi thong tin cac tai khoan
          printTable(listTable); //Hien thi cac ban choi
          //DONE HANDING DATA TO SEND-------------------//

          if (ret <= 0)
          {
            FD_CLR(sockfd, &allset);
            close(sockfd);
            client[i] = -1;
          }
        }

        if (--nready <= 0)
          break; /* no more readable descriptors */
      }
    }
  }

  return 0;
}
