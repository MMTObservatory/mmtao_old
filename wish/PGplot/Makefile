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

LIB = lib_PGplot

LIST = 	PGplot_Cmd.c \
	PGplot_ColorTable.c \
	PGplot_Open.c \
	PGplot_SetupInfo.c \
	PGplot_SlopesCalculate.c \
	PGplot_SlopesSetup.c \
	PGplot_SlopesUpdate.c \
	PGplot_SpotsCalculate.c \
	PGplot_SpotsPattern.c \
	PGplot_SpotsSetup.c \
	PGplot_SpotsUpdate.c \
	PGplot_TestDraw.c

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
