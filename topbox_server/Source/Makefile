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

LIST = 	Source_MVP.c \
	Source_ReadMVP.c \
	Source_RelativeMVP.c \
	Source_Stage.c \
	Source_StageRead.c \
	Source_StageRelativeMove.c \
	Source_StatusMVP.c

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
#
# main() routines to start the routines above
#

source_server: Source_Server.c \
	lib_Source.a \
	../MVP/lib_MVP.a \
	../Socket/lib_Socket.a \
	../Status/lib_Status.a
	gcc $(CFLAGS) Source_Server.c lib_Source.a $(LD_FLAGS) ../MVP/lib_MVP.a ../Socket/lib_Socket.a ../Status/lib_Status.a -o source_server
