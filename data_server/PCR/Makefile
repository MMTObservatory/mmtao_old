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

LIB = lib_PCR

LIST = 	PCR_FindSOF.c \
	PCR_GetDM.c \
	PCR_GetWFSC.c \
	PCR_Process.c \
	PCR_SendCmd.c \
	PCR_SendCmd_NoReply.c \
	PCR_SendCmd_DM.c \
	PCR_SendHelp.c \
	PCR_SendFloatVector.c \
	PCR_SendIntVector.c \
	PCR_SendShortVector.c \
	PCR_SendSlopeOffsets.c

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
