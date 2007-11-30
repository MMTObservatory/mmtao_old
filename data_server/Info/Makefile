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

LIB = lib_Info

LIST = 	Info_FindParameter.c \
	Info_Master.c \
	Info_Process.c \
	Info_ReadFile.c \
	Info_ReadLine.c \
	Info_SeeingPositions.c \
	Info_SendAll.c \
	Info_SendHelp.c \
	Info_SendSeeing.c \
	Info_SetFlagPointers.c \
	Info_SetParameter.c

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
