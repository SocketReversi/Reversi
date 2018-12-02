typedef enum {
  USER,
  PASS,
  REGISTER,
  LOGOUT,
  CREATE,
  JOIN,
  MOVE,
  LEAVE,
} OPCODE;

typedef enum {
  USER_ERROR,
  PASS_ERROR,
  JOIN_ERROR,
  MOVE_ERROR,
} ERROR;

typedef struct message {
  int opcode;
  char message[50];
} message;


void processData(char *in, char *out);
int receiveData(int s, char *buff, int size, int flags);
int sendData(int s, char *buff, int size, int flags);