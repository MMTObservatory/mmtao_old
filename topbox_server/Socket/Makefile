##############################################################
#
#  Makefile to create a library of functions for the PCR DServ routine
#
#  This file contains only varaibles which are specific to
#    the compilation of the files in this subdirectory.
#
#  General AO compilation parameters are held in
#	../include/Make_Flags
#	../include/Make_Dependencies
#
#-------------------------------------------------------------
#
#  Written  19nov05  DLM  Started with Makefile from MMTAO GUI
#
##############################################################
#
# Application dependent variables
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# List of functions to include in library
#

LIB = lib_Socket

LIST =	Socket_Close.c \
	Socket_ConvertFloat.c \
	Socket_Flush.c \
	Socket_OpenServer.c \
	Socket_OpenClient.c \
	Socket_Read.c \
	Socket_ReadRaw.c \
	Socket_Setup.c \
	Socket_StringLowerCase.c \
	Socket_StringRead.c \
	Socket_StringTrim.c \
	Socket_StringWrite.c \
	Socket_Write.c

#
# ** Edit only variables above this line **
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Compiler specifics
#
include ../include/Topbox_Make_Flags
#
# Compile and archive functions
#
include ../include/Topbox_Make_Dependencies
