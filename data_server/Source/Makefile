################
#
#  Makefile to create a library of functions for the PCR DServ routine
#
#  This file contains only varaibles which are specific to
#    the compilation of the files in this subdirectory.
#
#  General AO compilation parameters are held in
#	../include/Make_Flags
#	../include/Make_Dependenies
#
#-------------------------------------------------------------
#
#  Written  19nov05  DLM  Started with Makefile from MMTAO GUI
#
################
#
# Application dependent variables
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# List of functions to include in library
#

LIB = lib_Source

LIST = 	Source_Start.c \
	Source_Stream_DM.c \
	Source_Stream_WFSC.c

#
# ** Edit only variables above this line **
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Compiler specifics
#
include ../include/PCR_Make_Flags
#
# Compile and archive functions
#
include ../include/PCR_Make_Dependencies
#
# main() routines to start the routines above
#
#Source: Source_Test.c lib_Source.a
#	gcc $(CFLAGS) Source_Test.c lib_Source.a ../Socket/lib_Socket.a -o source

dm: dm.c lib_Source.a ../Socket/lib_Socket.a
	gcc $(CFLAGS) dm.c lib_Source.a $(LD_FLAGS) ../Socket/lib_Socket.a -o dm

wfsc: wfsc.c lib_Source.a ../Socket/lib_Socket.a
	gcc $(CFLAGS) wfsc.c lib_Source.a $(LD_FLAGS) ../Socket/lib_Socket.a -o wfsc

cmd: cmd.c lib_Source.a ../Socket/lib_Socket.a
	gcc $(CFLAGS) cmd.c lib_Source.a $(LD_FLAGS) ../Socket/lib_Socket.a -o cmd

error: error.c lib_Source.a ../Socket/lib_Socket.a
	gcc $(CFLAGS) error.c lib_Source.a $(LD_FLAGS) ../Socket/lib_Socket.a -o error
