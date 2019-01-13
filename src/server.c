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
  int nready, client[FD_SETSIZE];
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
          sendBuff = groupClient(rcvBuff, listTable, sockfd);
          
          if(sendBuff == NULL){
            sendBuff = handleRequest(rcvBuff,listUser);
          }
          if(sendBuff == NULL){
            sendBuff = playGame(rcvBuff, listTable, sockfd);
          }
          printf("Ma tra ve : %d\n",sendBuff->opcode );
          //SEND DATA TO CLIENT--------------------------//

          if(   sendBuff->opcode == LOGIN_SUCCESS||
                sendBuff->opcode == LOGIN_FAIL||

                sendBuff->opcode == REGISTER_SUCCESS||
                sendBuff->opcode == REGISTER_FAIL||

                sendBuff->opcode == LOGOUT_SUCCESS||
                sendBuff->opcode == LOGOUT_FAIL||
                sendBuff->opcode == JOIN_FAIL||
                sendBuff->opcode == LEAVE_FAIL||
                
                sendBuff->opcode == PLAY_FAIL||
                sendBuff->opcode == MOVE_FAIL||
                sendBuff->opcode == CHECK){
            printTable(listTable);
            ret = sendData(sockfd, sendBuff, sizeof(Request), 0);

          }else if(sendBuff->opcode == CREATE_SUCCESS){
            listTable = createTable(listTable, sockfd);
            printTable(listTable);
            ret = sendData(sockfd, sendBuff, sizeof(Request), 0);
          }

          else if(sendBuff->opcode == LEAVE_SUCCESS || sendBuff->opcode == END_GAME){
            table *node = findWithID(listTable, sockfd)->data;
            sendData(node->master, sendBuff, sizeof(Request), 0);
            sendData(node->guest, sendBuff, sizeof(Request), 0);
            listTable = leaveTable(listTable, sockfd);
            printTable(listTable);
          }
          else if(sendBuff->opcode == PLAY_SUCCESS){
            display(sendBuff->board);
            printTable(listTable);
            ret = sendData(sockfd, sendBuff, sizeof(Request), 0);
          }
          else {
            printTable(listTable);
            printf("%s\n",sendBuff->message );
            int type = Player(listTable, sockfd); 
            printf("TYPE : %d\n", type);
            table *node = (table *)findWithID(listTable, sockfd)->data;
            printf("Master: %d - Guest: %d\n",node->master,node->guest);
            if(type == MASTER){
                printf("Ma guest : %d\n", node->guest);
                sendData(node->guest, sendBuff, sizeof(Request), 0);
            }
            else if(type == GUEST){
              printf("Ma master : %d\n", node->master);
              sendData(node->master, sendBuff, sizeof(Request), 0);
            }else{
              ret = sendData(sockfd, sendBuff, sizeof(Request), 0);
            }
          }
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
