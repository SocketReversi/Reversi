#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5500
#define BUFF_SIZE 1024

int main(int argc, char *argv[]){
	int client_sock;
	char buff[BUFF_SIZE];
	struct sockaddr_in server_addr; /* server's address information */
	int msg_len, bytes_sent, bytes_received;
	
	if(argc !=3){
		printf("PARAMETER INVALID!\n");
		return 0;
	}

	//Step 1: Construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);
	
	//Step 2: Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	//Step 3: Request to connect server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nERROR!CANNOT CONNECT TO CLIENT! CLIENT EXIT IMEDIATELY! ");
		return 0;
	}
	//Step 4: Communicate with server			
	while(1){
		//receive echo reply
		bytes_received = recv(client_sock, buff, BUFF_SIZE-1, 0);
		if(bytes_received <= 0){
			printf("\nCANNOT RECEIVE DATA FROM SERVER!\n");
			break;
		}
		
		buff[bytes_received] = '\0';
		printf("----------------\n");
		printf("SERVER-MESSAGE	: [%s]\n", buff);
		
		//send message
		printf("\nINPUT	:");
		memset(buff,'\0',(strlen(buff)+1));
		fgets(buff, BUFF_SIZE, stdin);
		msg_len = strlen(buff);
		if (msg_len == 1){
			printf("EXIT!\n");
			break;
		}
		
		bytes_sent = send(client_sock, buff, msg_len, 0);
		if(bytes_sent <= 0){
			printf("\nCONNECTION CLOSED!\n");
			break;
		}
	}
	
	//Step 4: Close socket
	close(client_sock);
	return 0;
}