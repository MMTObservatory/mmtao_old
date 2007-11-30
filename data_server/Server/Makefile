##############################################################
#
# Adaptive Optics (AO) Makefile for VxWorks routines
#
# This file contains only varaibles which are specific to
#   the compilation of the files in this subdirectory.
#
# General AO compilation parameters are held in
#	../include/VXW_Make_Flags
#	../include/VXW_Make_Dependenies
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

LIB = lib_Server

LIST =	Server_Com.c \
	Server_StringLowerCase.c \
	Server_StringRead.c \
	Server_StringTrim.c \
	Server_StringWrite.c

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
