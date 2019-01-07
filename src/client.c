#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "../libs/account.h"
#include "../libs/valid.h"
#include "../libs/request.h"
#include "../libs/clientHandle.h"
#include "../libs/reversi.h"

int main(int argc, char *argv[])
{
  paramsClientValid(argc);
  //set server address
  char SERVER_ADDR[20];
  strcpy(SERVER_ADDR, argv[1]);
  int SERVER_PORT = atoi(argv[2]);
  int client_sock;
  char buff[BUFF_SIZE];
  struct sockaddr_in server_addr; /* server's address information */
  int msg_len, bytes_sent, bytes_received;

  int responseCode;
  //Step 1: Construct socket
  client_sock = socket(AF_INET, SOCK_STREAM, 0);

  //Step 2: Specify server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

  //Step 3: Request to connect server
  if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
  {
    printf("\nError!Can not connect to sever! Client exit imediately! ");
    return 0;
  }

  //Step 4: Communicate with server

  //send message
  while (1)
  {
    Request *request = clientHandle();

    bytes_sent = sendData(client_sock, request, sizeof(Request), 0);

    if (bytes_sent <= 0){
      perror("\nError: ");
      break;
    }
    else{
      printf("\n");
    }

    //receive echo reply
    bytes_received = receiveData(client_sock, request, sizeof(Request), 0);
    display(request->board);
    if (bytes_received < 0){
      perror("\nError: ");
      break;
    }
    else if (bytes_received == 0){
      printf("Connection closed.\n");
      break;
    }

    if(renderMessage(request) == WAIT){
      //receive echo reply
      bytes_received = receiveData(client_sock, request, sizeof(Request), 0);
      display(request->board);
      if (bytes_received < 0){
        perror("\nError: ");
        break;
      }
      else if (bytes_received == 0){
        printf("Connection closed.\n");
        break;
      }
    }
  }

  //Step 4: Close socket
  close(client_sock);

  return 0;
}