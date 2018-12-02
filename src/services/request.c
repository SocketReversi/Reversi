#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

void processData(char *in, char *out)
{
  strcpy(out, in);
}

int receiveData(int s, char *buff, int size, int flags)
{
  int n;
  n = recv(s, buff, size, flags);
  if (n < 0)
    perror("Error: ");
  return n;
}

int sendData(int s, char *buff, int size, int flags)
{
  int n;

  n = send(s, buff, size, flags);
  if (n < 0)
    perror("Error: ");
  return n;
}