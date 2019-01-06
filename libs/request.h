/**
 ** Client request opcode 
 **/
typedef enum {
  LOGIN = 1,
  REGISTER,
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
  LOGIN_SUCCESS,
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
  LOGIN_FAIL,
  REGISTER_FAIL,
  LOGOUT_FAIL,
  NOT_LOGIN,
  CANT_JOIN
} ERROR;

// Request struct
typedef struct Request {
  int opcode;
  char message[50];
  char username[50];
  char password[50];
  int doc;
  int ngang;
} Request;

// Method
void processData(char *in, char *out);
int receiveData(int s, Request *buff, int size, int flags);
int sendData(int s, Request *buff, int size, int flags);