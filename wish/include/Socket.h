/*

  Generic socket definitions

  Written  28dec04  DLM

*/

/*
  Maximum socket I/O line size
*/
#define SOCKET_NAME_LENGTH 80
#define SOCKET_MAXLINE 256

/*
  Delay time for motor response
*/
#define SOCKET_DELAY_S 1
#define SOCKET_DELAY_U 0

/*
  Socket information structure
*/
typedef struct _Socket_Info {
  char motor[SOCKET_NAME_LENGTH];   // motor name
  char name[SOCKET_NAME_LENGTH];    // drive name
  char address[SOCKET_NAME_LENGTH]; // motor address
  int port;                         // motor port
  int driveid;                      // drive number (0 for RS232)
  int sockfd;                       // socket fd
  int connected;                    // socket connected?
  int prev_connected;               // store the previous socket connected value
  int error;                        // error?
  char in_line[SOCKET_MAXLINE];     // line for input
  char out_line[SOCKET_MAXLINE];    // line for output
} Socket_Info;

/*
  Socket routines
*/
int Socket_Close (Socket_Info *Info, int debug);
int Socket_Open (Socket_Info *Info, int debug);
