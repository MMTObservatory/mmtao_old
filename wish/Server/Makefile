################
#
#  Makefile to create a library of functions for the MMTAO GUI
#
#  These routines are called by the recompiled TclTk wish shell
#
#  Written   4apr00  DLM
#
#  Altered  24feb03  DLM  Added make depends
#                         Replaced implicit .c.a: with .c.o: and .o.a:
#                           to be able to use the depends file.
#                         Puts the general make commands (flags and dependancies)
#                           into files to be included.
#
################
#
# Application dependent variables
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# List of functions to include in library
#

LIB = lib_Server

LIST = 	Server_Close.c \
	Server_Comm.c \
	Server_GuiUpdate.c \
	Server_Open.c \
	Server_SetupState.c \
	Server_StringLowerCase.c \
	Server_ZeroState.c

#
# ** Edit only variables above this line **
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Compiler specifics
#

include $(PCR_HOME)/wish/include/PCR_Make_Flags

#
# Compile and archive functions
#

include $(PCR_HOME)/wish/include/PCR_Make_Dependancies
