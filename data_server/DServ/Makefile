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

LIB = lib_DServ

LIST = 	DServ_GetSingle.c \
	DServ_InitWFSC.c \
	DServ_InitDM.c \
	DServ_InitSlopes.c \
	DServ_Master.c \
	DServ_Process.c \
	DServ_SendHelp.c \
	DServ_Stream.c \
	WFSC_GetSingle.c \
	WFSC_Master.c \
	WFSC_Process.c

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
dserv: serv.c lib_DServ.a ../Socket/lib_Socket.a
	gcc $(CFLAGS) $(LD_FLAGS) serv.c lib_DServ.a ../PCR/lib_PCR.a ../Info/lib_Info.a ../Socket/lib_Socket.a -o dserv
