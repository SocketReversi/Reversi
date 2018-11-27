CC = gcc
LIBS =  -lm 

server client client2 : server.c client.c client2.c
	${CC} -o server server.c
	${CC} -o client client.c
	${CC} -o client2 client2.c

clean:
	rm -f *.o *~