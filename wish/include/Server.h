/*
  Server information
*/

#define SERVER_LENGTH 80

/*
  Delay time for motor response
*/
#define SERVER_DELAY_S 5
#define SERVER_DELAY_U 0

/*
  Structure to hold Server information and communication
*/
typedef struct _Server_Info {
  char name[SERVER_LENGTH];       // server name
  char type[SERVER_LENGTH];       // server type
  char address[SERVER_LENGTH];    // server address
  int  port;                      // server port
  int  error;                     // is there an error with the server
  int  connected;                 // are we connected to server
  int  fd;                        // server file descripter
  char out_line[SERVER_LENGTH];   // line for output
  char *in_line;                  // line for input
  int  nBytes;                    // number of bytes in reply
} Server_Info;

/*
  Prototypes
*/
int Server_Close (Server_Info *Info, int debug);
int Server_Flush ( Server_Info *Info );
int Server_GuiUpdate( Tcl_Interp *interp, Server_Info *Info, Gui_Variables *Gui);
int Server_Open (Server_Info *Info, int debug);
int Server_Read( Server_Info *Info, int nBytes );
int Server_SetupState( Server_Info *Info, char *name, int debug);
int Server_StringLowerCase( char *request);
int Server_Write( Server_Info *Info );
int Server_ZeroState( Server_Info *Info);
