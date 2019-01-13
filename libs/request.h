/**
 ** Client request opcode 
 **/
typedef enum {
  LOGIN = 1,
  REGISTER,
  LOGOUT,
  CREATE,
  JOIN,
  CHECK,
  CHAT,
  LEAVE,
  PLAY,
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
  PLAY_SUCCESS,
  MOVE_SUCCESS,
  END_GAME,
} SERVER_RESPONSE;

/**
 ** Error opcode
 **/
typedef enum {
  LOGIN_FAIL = 40,
  REGISTER_FAIL,
  LOGOUT_FAIL,
  CREATE_FAIL,
  JOIN_FAIL,
  LEAVE_FAIL,
  PLAY_FAIL,
  MOVE_FAIL,
} ERROR;

// Request struct
typedef struct Request {
  int opcode;
  int board[8][8]; //Du lieu luu CO PHO
  int turn;        //du lieu de xac dinh luot choi
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