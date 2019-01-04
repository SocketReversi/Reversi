CFLAGS = -c
CC = gcc
LIBS =  -lm 

server client: server.o lib.o client.o
	${CC} server.o lib.o -o server
	${CC} client.o lib.o -o client

client.o: client.c
	${CC} ${CFLAGS} client.c

server.o: server.c
	${CC} ${CFLAGS} server.c

lib.o: lib.c
	${CC} ${CFLAGS} lib.c

clean:
	rm -f *.o *~