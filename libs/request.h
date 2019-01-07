/**
 ** Client request opcode 
 **/
typedef enum {
  LOGIN = 1,
  REGISTER,
  LOGOUT,
  CREATE,
  JOIN,
  LEAVE,
  MOVE,
} CLIENT_REQUEST;

/**
 ** Server response opcode
 **/
typedef enum {
  LOGIN_SUCCESS = 20,
  REGISTER_SUCCESS,
  LOGOUT_SUCCESS,
  CREATE_SUCCESS,
  JOIN_SUCCESS,
  LEAVE_SUCCESS,
  MOVE_SUCCESS,
} SERVER_RESPONSE;

/**
 ** Error opcode
 **/
typedef enum {
  EMPTY = 40,
  LOGIN_FAIL,
  REGISTER_FAIL,
  LOGOUT_FAIL,
  CREATE_FAIL,
  JOIN_FAIL,
  MOVE_FAIL,
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