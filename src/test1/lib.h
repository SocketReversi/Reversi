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
