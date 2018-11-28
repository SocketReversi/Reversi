CC = gcc
LIBS =  -lm 

server client: server.c client.c
	${CC} -o server server.c
	${CC} -o client client.c

clean:
	rm -f *.o *~