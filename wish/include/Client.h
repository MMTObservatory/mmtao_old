/****************************
  Client information
****************************/

/*
  Delay time for server to respond
*/
#define CLIENT_DELAY_S 1
#define CLIENT_DELAY_U 0

/*
  Structure to hold Client information
*/
typedef struct _Client_Info {
  char name[STRING_LENGTH];       // server name
  char address[STRING_LENGTH];    // server address
  int  port;                      // server port
  int  error;                     // is there an error with the server
  int  connected;                 // are we connected to server
  int  fd;                        // server file descripter
} Client_Info;

/*
  Structure to hold pointers to variables linked between C and TclTk
    used to update values on the GUI when they are change in a C routine.
  This requires the storage of name of the TclTk variable and the pointer
    to the memory location of this variable.
*/
typedef struct _Int_Info {
  char name[STRING_LENGTH];
  int *ptr;
} Int_Info;

typedef struct _Double_Info {
  char name[STRING_LENGTH];
  double *ptr;
} Double_Info;

typedef struct _Gui_Variables {
  Int_Info connected;
  Int_Info error;
} Gui_Variables;

/* Prototypes */
int Client_Close(Client_Info *Info, int debug);
int Client_EndOfData( char *input );
int Client_Flush ( register int fd);
int Client_GuiUpdate( Tcl_Interp *interp, Client_Info *Info, Gui_Variables *Gui);
int Client_Open( Client_Info *Info, int debug);
int Client_Read( register int fd, register char *ptr, register int maxlen );
int Client_ReadLine( register int fd, register char *ptr, register int maxlen );
int Client_ReadToEOF( Client_Info *Info, char *Client_Reply, char *errorMsg, int debug);
int Client_SetupInfo( Client_Info *Info, Gui_Variables *Gui, char *name,
		      int *connected, int *error);
int Client_Write( register int fd, register char *ptr, register int nbytes);

/* Client_Error.c */
int Client_ParameterError( Tcl_Interp *interp, char *type, char *routine, char *request, 
			int required, int found, char *returnString, long debug);
int Client_RoutineError( Tcl_Interp *interp, char *type, char *routine, char *request,
		      char *message, char *returnString, long debug);
int Client_ConnectionWarning(  Tcl_Interp *interp, char *type, char *routine, char * name,
			    char *request, char *returnString, long debug);
