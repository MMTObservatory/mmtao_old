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
#  Modified 24aug07  tjt  Started with Makefile from the MVP directory
#
################
#
# Application dependent variables
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# List of functions to include in library
#

LIB = lib_Stage

LIST = 	Stage_Enable.c \
	Stage_Disable.c \
	Stage_Home.c \
	Stage_Looper.c \
	Stage_MoveAbsolute.c \
	Stage_MoveRelative.c \
	Stage_Position.c \
	Stage_Process.c \
	Stage_PrintDevice.c \
	Stage_Read.c \
	Stage_SetHomeVar.c \
	Stage_SetSpeed.c \
	Stage_SetZero.c \
	Stage_Status.c \
	Stage_Write.c

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
