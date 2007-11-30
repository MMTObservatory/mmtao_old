################
#
#  Makefile to create a library of functions for the topbox filter wheel
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
#  Modified 29aug07  tjt  Started with Makefile from ../Stage
#
################
#
# Application dependent variables
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# List of functions to include in library
#

LIB = lib_Fwheel

LIST = \
	Fwheel_Home.c \
	Fwheel_IO.c \
	Fwheel_Looper.c \
	Fwheel_Move.c \
	Fwheel_PrintDevice.c \
	Fwheel_Process.c

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
