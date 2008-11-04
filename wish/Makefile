################
#
#  Generic Makefile to create a library of functions, compile
#    a main routine and link this main routine to the library
#    just created and to required system libraries.
#
#  Written  4 April 2000  DLM
#
#  Altered  30nov07  DLM  Removed all the extra directory compilations
# 			    specifically the Topbox routines (FSM, OAP, etc)
#                           that are no longer neede now that the topbox_server
#                           threaded C program now works.
#
################
#
# Application dependent variables
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Main program: will result in executable $(NAME)

NAME = PCR_Wish

#
# List of subdirectories containing needed libraries (routines)
#

#  NOTE: VoltMeter directory is currently not included in
#        the compilation of PCR_Wish.  The VoltMeter
# 	 GUI is about 90% done.

SUB_DIRS = BG Client Cmd DClient Hex Info Mod \
	PCR PGplot Server Socket Static Status TCS \
	Topbox Utility Science TSS

LINK_LIST = ./BG/lib_BG.a \
            ./DClient/lib_DClient.a \
            ./Hex/lib_Hex.a \
            ./Info/lib_Info.a \
            ./Mod/lib_Mod.a \
            ./PGplot/lib_PGplot.a \
            ./Socket/lib_Socket.a \
            ./Static/lib_Static.a \
            ./PCR/lib_PCR.a \
            ./Status/lib_Status.a \
            ./DClient/lib_DClient.a \
            ./TCS/lib_TCS.a \
            ./Topbox/lib_Topbox.a \
	    ./Utility/lib_Utility.a \
            ./Client/lib_Client.a \
            ./Server/lib_Server.a \
            ./Science/lib_Science.a \
            ./TSS/lib_TSS.a \
            ./Cmd/lib_Cmd.a


LIST = $(NAME).c

#
# ** Edit only variables above this line **
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Compiler specifics
#

include $(PCR_HOME)/wish/include/PCR_Make_Flags

#
# Compile and link main routine
#

$(NAME).x: subdirectories $(NAME).o
	@echo ' '
	@echo '   linking '$(NAME)''
	@echo ' '
	@$(LINKER) $(CFLAGS) -o $(NAME) \
		$(NAME).o \
		$(LINK_LIST) \
		$(LD_FLAGS)

$(NAME).o: $(NAME).c $(ADD_INS)
	$(CC) $(CFLAGS) -c $(NAME).c

#
# Compile and archive functions
#

include $(PCR_HOME)/wish/include/PCR_Make_Dependancies

#
# Change to each subdirectory listed in SUB_DIR and
#  execute Makefile
#

subdirectories:
	@for i in $(SUB_DIRS); do \
		make -C $$i --no-print-directory; \
	done

clean_all:
	@${RM} $(NAME).o $(NAME) depends
	@for i in $(SUB_DIRS); do \
		echo "cleaning $$i"; \
		make -C $$i --no-print-directory clean; \
	done
