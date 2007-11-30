/*==============================================*
**
** This C program creates Tcl-Tk callable procedures.
**
** This is done by writting C routines with the proper
** Tcl structures, functions and includes, compiling
** these C routines and linking them to the Tcl and Tk libraries
** to create a new TclTk interpreter.  This new
** interpreter is used in place of the wish, with all
** the previous Tcl procedure available as well as your
** new procedure.
**
** A good overview is given in Foster-Johnson, "Graphical
** Applications with Tcl & Tk, Second Edition", M & T Books,
** 1997, Chapter 16.
**
** A very good resourse on the web, see the TclTk
** Home page from Scriptics at 
**     http://dev.scriptics.com
** with an especially useful set of manual pages at
**     http://dev.scriptics.com/man/tcl8.1/contents.htm
**
**
** Written:  11 April 2000 DLM
**
**==============================================*/

#include "PCR_Wish.h"
#include "Server_Ports.h"
#include "Client.h"
#include "Server.h"

/*
  Prototypes for client routines that connect to TCS, Hexapod
    and VME (PCR) servers
 */
#include "Client_Prototypes.h"
#include "Client_Globals.h"

/*
  Prototypes for PGplot routines
*/
#include "PGplot.h"
#include "PGplot_Prototypes.h"
#include "PGplot_Globals.h"

/*
  Prototypes for VoltMeter routines
    NOTE: VoltMeter_Cmd is currently not included in
         the compilation of PCR_Wish.  The VoltMeter
	 GUI is about 90% done.  30nov07 DLM

#include "VoltMeter_Prototypes.h"
#include "VoltMeter_Globals.h"
*/

/*
  Prototypes of routines used by the AO GUI.  They have the form of
    *_Cmd()
*/
#include "Cmd_Prototypes.h"
#include "Cmd_Globals.h"

/*
  Create global variables to pass Server address from C to TclTk
*/
#include "Server_Globals.h"

/*==============================================*
**
** Include new procedure in TckTk interpreter
** ------------------------------------------
**
** This routine defines the new procedures in the
** new Tcl interpreter.  A Tcl_CreateCommand (String-
** based approach) or Tcl_ObjCreateCommand (Object-
** based approach) routine must be called for each
** of the new commands.
** 
** For example:
** 
**     (String-based approach)
**   Tcl_CreateCommand(interp,"proc_name",(Tcl_CmdProc*) routine_name,
** 		    (ClientData) NULL,(Tcl_CmdDeleteProc*) NULL);
** 
**     (Object-based approach)
**   Tcl_CreateObjCommand(interp,"proc_name",(Tcl_ObjCmdProc*) routine_name,
** 		    (ClientData) NULL,(Tcl_CmdDeleteProc*) NULL);
** 
** where:
**   proc_name    - name of the procedure which will be available in Tcl
**   routine_name - name of the C routine defining the prodedure
** 
** Example of how to set variable so they are seen here 
**       as well in the TclTk scripts
**
**    Tcl_SetVar(interp, "WFC_port_number", "5100", 0);
**    printf(" new port = %d\n",WFC_port_number);
**    Tcl_SetVar(interp, "WFC_address", "new value", 0);
**    printf(" new address = %s\n",WFC_address);
**
**
**==============================================*/

int Tcl_AppInit(Tcl_Interp* interp)
{
  long status;
  status = Tcl_Init(interp);
  if (status != TCL_OK) return TCL_ERROR;

  status = Tk_Init(interp);
  if (status != TCL_OK) return TCL_ERROR;

  status = Tix_Init(interp);
  if (status != TCL_OK) return TCL_ERROR;

  status = Tkpgplot_Init(interp);
  if (status != TCL_OK) return TCL_ERROR;

  /*
    This package must be included in order for the built in Tcl socket communication
      procedure to work properly when the TclTk shell (wish) is re-compiled.  If this package
      is not started then the server routine hangs when a socket is set to listen.  It
      was determined that this package is need by examining the original Tcl_AppInit routine
      in ~/caao/tcltk/lib/tk8.3/tkAppInit.c.  It is not clear to me why this package is
      require for to use sockets, and I have not found and other options which require
      this to be started.  25oct01 DLM
  */
  Tcl_StaticPackage(interp, "Tk", Tk_Init, Tk_SafeInit);

  /*
    =================================
    Link Tcl variables to C variables
    =================================
  */

  /*
    Link client Tcl and C variables
  */
  #include "Client_Link.h"

  /*
    Link client Tcl and C variables
  */
  #include "PGplot_Link.h"

  /*
    Link AO GUI Tcl and C variables
  */
  #include "Cmd_Link.h"

  /*
    Link AO GUI Tcl and C variables
      Currently not included!!  30nov07  DLM

  #include "VoltMeter_Link.h"
  */

  /*
    Link AO GUI Tcl and C variables
  */
  #include "Server_Link.h"

  return TCL_OK;
}

int main(int argc, char** argv)
{
  Tk_Main(argc, argv,Tcl_AppInit);
  return 0;
}

