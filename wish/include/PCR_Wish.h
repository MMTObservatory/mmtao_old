/*
  Includes and definitions to be include in all
    AO_wish routines

  Written  8jan04  DLM
*/

// Standard C include file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <tcl.h>
#include <tk.h>
#include <tix.h>

#include <time.h>

#include "tkpgplot.h"
#include "cpgplot.h"

/************************************
 *   Server Structure Information *
 ************************************/

//#include "Server.h"


// Maximum line lengths

#define IO_LENGTH       512
#define STRING_LENGTH    80

// Length of string of string return by the TCS

#define TCS_LENGTH  8192

// Length of string of string return by the Hexapod

#define HEXAPOD_LENGTH  8192

// Length of string of string return by Client_ReadToEOF

#define CLIENT_REPLY_LENGTH  8192

// Uncomment if not at the MMT

#define AT_MMT
