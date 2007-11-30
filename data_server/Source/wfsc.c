
#include "PCR.h"

int Source_Start( char *name, char *type, int size, long debug );

int main(int argc, char *argv[])
{
  int status;
  char type[STRING_LENGTH];
  int size;

  if ( argc > 1 ) {
    /*
      Check to see if a type of data has been requested
    */
    if ( !strncmp( argv[1], "test", strlen("test") ) ||
	 !strncmp( argv[1], "bg", strlen("bg") ) ||
	 !strncmp( argv[1], "real", strlen("real") ) ) {
      strcpy ( type, argv[1]);
      /*
	Check to see if a size has been requested
      */
      if ( argc > 2 ) {
	size = atoi(argv[2]);
	if ( (size != 24) &&
	     (size != 72) ) {
	  /*
	    Set default value for size
	  */
	  size = 24;
	}
      } else {
	/*
	  Set default value for size
	*/
	size = 24;
      }

    } else {

      /*
	Set default value for type
      */
      strcpy( type, "real");
      size = 24;
    }

  } else {
      
    /*
      Set default value for type and size
    */
    strcpy( type, "real");
    size = 24;
  }

  status = Source_Start( "WFSC", type, size, 1);
  return (0);
}
