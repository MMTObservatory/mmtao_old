/*
  Define the prototypes for the Socket routines

  This should be included in all routines which use
    and Socket_* routines

  Written 19nov05  DLM
*/

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <unistd.h>

/*
  Maximum socket I/O line size
*/
#define SOCKET_NAME_LENGTH 80
#define SOCKET_MAXLINE 512

/*
  Delay time for motor response
*/
#define SOCKET_DELAY_S 1
#define SOCKET_DELAY_U 0

/*
  Socket information structure
*/
typedef struct _Socket_Info {
  char name[SOCKET_NAME_LENGTH];     // Name of this client or server
  char address[SOCKET_NAME_LENGTH];  // Address of server this client will connnect (localhost if server)
  int port;                          // Port to connect to (client) or to listen to (server)
  int sockfd;                        // File descriptor of connection
  int connected;                     // Is there connection?
  int error;                         // Is there an error?
  pthread_mutex_t lock;              // Lock variable used to by pthread_mutex_lock(lock) to make
                                     //   sure only one thread communicates on a socket connect at
                                     //   any one time.
} Socket_Info;

/* Socket Routine Prototypes */
int Socket_Close( Socket_Info *Info, int debug);
int Socket_ConvertFloat( float *raw, float *flipped, int num);
int Socket_Flush( int fd);
int Socket_OpenClient( Socket_Info *Info, int debug);
int Socket_OpenServer( Socket_Info *Info, int debug);
int Socket_Read( int sockfd, char *buffer, int nBytes);
int Socket_Setup( Socket_Info *Info, char *name, int debug);
int Socket_StringLowerCase( char *request);
int Socket_StringRead( int fd, char *buffer);
int Socket_StringTrim( char *request);
int Socket_StringWrite( register int fd, register char *ptr, register long nbytes);
int Socket_Write( register int fd, register char *ptr, register long nbytes);

#endif /* _SOCKET_H_ */
