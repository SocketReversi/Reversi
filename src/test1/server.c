#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/select.h> 
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "lib.h"
#include "acc_libs.h"
#include "game_libs.h"

#define PORT 5500   /* Port that will be opened */ 
#define BACKLOG 20   /* Number of allowed connections */
#define BUFF_SIZE 1024

/* The processData function copies the input string to output */
void processData(char *in, char *out);

/* The recv() wrapper function*/
int receiveData(int s, char *buff, int size, int flags);

/* The send() wrapper function*/
int sendData(int s, char *buff, int size, int flags);

//reset buff 
int resetBuff(char *sendBuf,char *rcvBuf);

int main(int argc , char *argv[])
{
	int i, maxi, maxfd, listenfd, connfd, sockfd;
	int nready, client[FD_SETSIZE];
	ssize_t	ret;
	fd_set	readfds, allset;
	char sendBuff[BUFF_SIZE], rcvBuff[BUFF_SIZE];
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	tableGame table[MAX_TABLE];
	createTableList(table);
	printf("Khoi tao khong gian cac ban choi\n");

	if(argc !=2){
		printf("PARAMETER INVALID!\n");
		return 0;
	}

	//Step 1: Construct a TCP socket to listen connection request
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}

	//Step 2: Bind address to socket
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port 		 = htons(atoi(argv[1]));

	if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr))==-1){ /* calls bind() */
		perror("\nError: ");
		return 0;
	} 

	//Step 3: Listen request from client
	if(listen(listenfd, BACKLOG) == -1){  /* calls listen() */
		perror("\nError: ");
		return 0;
	}

	maxfd = listenfd;			/* initialize */
	maxi = -1;				/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++){
		client[i] = -1;
	}
					/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
	
	//Step 4: Communicate with clients
	while (1) {
		readfds = allset;		/* structure assignment */
		nready = select(maxfd+1, &readfds, NULL, NULL, NULL);
		if(nready < 0){
			perror("\nError: ");
			return 0;
		}
		
		if (FD_ISSET(listenfd, &readfds)) {	/* new client connection */
			clilen = sizeof(cliaddr);
			if((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0)
				perror("\nError: ");
			else{
				printf("You got a connection from %s[%d]\n", inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port)); /* prints client's IP */
				for (i = 0; i < FD_SETSIZE; i++)
					if (client[i] < 0) {
						client[i] = connfd;	/* save descriptor */
						break;
					}
				if (i == FD_SETSIZE){
					printf("\nToo many clients");
					close(connfd);
				}

				FD_SET(connfd, &allset);	/* add new descriptor to set */
				if (connfd > maxfd)
					maxfd = connfd;		/* for select */
				if (i > maxi)
					maxi = i;		/* max index in client[] array */

				if (--nready <= 0)
					continue;		/* no more readable descriptors */
			}
		}

		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			if ( client[i] < 0) //thay bang client[i] cho (sockfd = client[i])
				continue;
			if (FD_ISSET(client[i], &readfds)) {
				ret = receiveData(client[i], rcvBuff, BUFF_SIZE, 0);
				rcvBuff[ret-1]='\0';
				if (ret <= 0){
					FD_CLR(client[i], &allset);
					close(client[i]);
					client[i] = -1;
				}
				
				else {
					if(strcmp(rcvBuff,"create") == 0){
						int create = createTable(client[i],table);
						if(create == 0){
							printf("Tao phong choi that bai\n");
							processData("Tao phong that bai!",sendBuff);
							sendData(client[i], sendBuff, strlen(sendBuff), 0);
							printTable(table);
							continue;
						}
						printf("Da tao 1 ban choi\n");
								
					}else if(strcmp(rcvBuff,"join") == 0){
						int join = joinTable(client[i],table);
						if(join == 0){
							processData("Khong tham gia duoc!",sendBuff);
							sendData(client[i], sendBuff, strlen(sendBuff), 0);
							printTable(table);
							continue;
						}
					}else if(strcmp(rcvBuff,"leave") == 0){
						int find = findID(client[i] , table);
						//printf("client : %d\n",client[i]);
						if(find != EMPTY){
							processData("Ban choi da duoc huy!",sendBuff);
						}else{
							processData("Roi phong that bai!",sendBuff);
						}
							//printf("master:[%d]-client:[%d]\n",table[find].master,table[find].guest);
							sendData(table[find].master, sendBuff, strlen(sendBuff), 0);
							sendData(table[find].guest, sendBuff, strlen(sendBuff), 0);
							printTable(table);
							leaveTable(client[i],table);
						continue;
					}
					processData(rcvBuff, sendBuff);

					int id = findIDgamer(client[i],table);
					if(id > -1 && id < MAX_TABLE){
						if(table[id].guest != EMPTY)
							sendData(table[id].guest, sendBuff, strlen(sendBuff), 0);
						printTable(table);
					}
					else if(id >= MAX_TABLE){
						sendData(table[id-MAX_TABLE].master, sendBuff, strlen(sendBuff), 0);
						printTable(table);
					}
					resetBuff(sendBuff,rcvBuff);
					if (ret <= 0){
						FD_CLR(client[i], &allset);
						close(client[i]);
						client[i] = -1;
					}
				}

				if (--nready <= 0)
					break;		/* no more readable descriptors */
			}
		}
	}
	
	return 0;
}

void processData(char *in, char *out){
	strcpy (out, in);
}

int receiveData(int s, char *buff, int size, int flags){
	int n;
	n = recv(s, buff, size, flags);
	if(n < 0)
		perror("Error: ");
	return n;
}

int sendData(int s, char *buff, int size, int flags){
	int n;
	n = send(s, buff, size, flags);
	//printf("[%d]\n",s);
	if(n < 0)
		perror("Send Error: ");
	return n;
}

int resetBuff(char *sendBuf,char *rcvBuf){
	memset(sendBuf,'\0',(strlen(sendBuf)+1));
	memset(rcvBuf,'\0',(strlen(rcvBuf)+1));
}