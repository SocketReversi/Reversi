#define BUFF_SIZE 1024
#define CONTINUE 111
#define WAIT 112
#define RESET 113

void printHelp();
Request *clientHandle();
int renderMessage(Request *request);