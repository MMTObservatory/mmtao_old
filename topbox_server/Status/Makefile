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

LIB = lib_Status

LIST = 	Status_FindParameter.c \
	Status_InitFlags.c \
	Status_Master.c \
	Status_Process.c \
	Status_ReadFile.c \
	Status_ReadLine.c \
	Status_SendAll.c \
	Status_SendHelp.c \
	Status_SetFlagPointers.c \
	Status_SetFwheelPointers.c \
	Status_SetPowerPointers.c \
	Status_ZeroFlags.c

#	Status_SetParameter.c

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
