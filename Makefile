################
#
#  Generic Makefile to create a library of functions, compile
#    a main routine and link this main routine to the library
#    just created and to required system libraries.
#
#  Written  4 Feb 2002  DLM
#
################
#
# Application dependent variables
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# List of subdirectories containing needed libraries (routines)
#

SUB_DIRS = data_server \
	topbox_server \
	instrument_server \
	wish

#
# ** Edit only variables above this line **
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Change to each subdirectory listed in SUB_DIR and
#  execute Makefile
#

subdirectories:
	@for i in $(SUB_DIRS); do \
		make -C $$i --no-print-directory; \
	done

clean:
	@${RM} $(NAME).o $(NAME)
	@for i in $(SUB_DIRS); do \
		echo "cleaning $$i"; \
		make -C $$i --no-print-directory clean_all; \
	done
