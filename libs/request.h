/**
 ** Client request opcode 
 **/
typedef enum {
  LOGIN = 1,
  USER_LOGIN,
  PASS_LOGIN,
  REGISTER,
  USER_REGISTER,
  PASS_REGISTER,
  LOGOUT,
  JOIN,
  LEAVE,
  MOVE,
} CLIENT_REQUEST;

/**
 ** Server response opcode
 **/
typedef enum {
  SUCCESS = 20,
  CAN_PASS_LOGIN,
  LOGIN_SUCCESS,
  CAN_PASS_REGISTER,
  REGISTER_SUCCESS,
  LOGOUT_SUCCESS,
  GO_GROUP,
  CAN_LEAVE
} SERVER_RESPONSE;

/**
 ** Error opcode
 **/
typedef enum {
  EMPTY = 40,
  USER_NOT_EXIST,
  PASS_INVALID,
  USER_EXIST,
  NOT_LOGIN,
  CANT_JOIN
} ERROR;

// Request struct
typedef struct Request {
  int opcode;
  char message[50];
} Request;

// Method
void processData(char *in, char *out);
int receiveData(int s, Request *buff, int size, int flags);
int sendData(int s, Request *buff, int size, int flags);
void renderMessage(Request *request);