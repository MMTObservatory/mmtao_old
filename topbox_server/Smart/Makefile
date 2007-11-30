################
#
#  Makefile to create a library of functions for the topbox Smart motors
#
#  This file contains only variables which are specific to
#    the compilation of the files in this subdirectory.
#
#  General AO compilation parameters are held in
#	../include/Make_Flags
#	../include/Make_Dependenies
#
#-------------------------------------------------------------
#
#  Modified 29aug07  tjt  Started with Makefile from ../Fwheel
#
################
#
# Application dependent variables
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# List of functions to include in library
#

LIB = lib_Smart

LIST = \
	Smart_IO.c \
	Smart_Set.c \
	Smart_Move.c \
	Smart_Home.c \
	Smart_Looper.c \
	Smart_Process.c \
	Smart_Status.c


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
