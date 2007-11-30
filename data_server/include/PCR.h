/*
  Includes and definitions to be include in all
    PCR routines

  Written  19nov05  DLM
*/

// Standard C include file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <curses.h>


// Maximum line lengths

#define IO_LENGTH       512
#define STRING_LENGTH   120
#define NAME_LENGTH      10

// Uncomment if compiling on Mac OS X

//#define MAC
