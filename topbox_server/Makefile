################
#
#  Generic Makefile to create a library of functions, compile
#    a main routine and link this main routine to the library
#    just created and to required system libraries.
#
#  Written  20nov05  DLM  Started with Makefile for MMTAO GUI
#
################
#
# Application dependent variables
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Main program: will result in executable $(NAME)

NAME = Topbox_Server

#
# List of subdirectories containing needed libraries (routines)
#

SUB_DIRS = \
	Master \
	Fwheel \
	MVP \
	Power \
	Smart \
	Stage \
	Status \
	Socket \
	Source

LINK_LIST = $(foreach dir, $(SUB_DIRS), ./$(dir)/lib_$(dir).a )

#
# ** Edit only variables above this line **
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# LIST is needed for make depends to work
#
LIST = $(NAME).c
#
# Compiler specifics
#
include ./include/Topbox_Make_Flags
#
# Compile and link main routine
#
$(NAME).x: subdirectories main $(NAME).o
	@echo ' '
	@echo '   linking '$(NAME)''
	@echo ' '
	@$(LINKER) $(CFLAGS) -o topbox_server \
		$(NAME).o \
		$(LINK_LIST) \
		$(LD_FLAGS)
#
$(NAME).o: $(NAME).c $(ADD_INS)
	$(CC) $(CFLAGS) -c $(NAME).c

#
# Compile and archive functions
#
include ./include/Topbox_Make_Dependencies
#

subdirectories:
	@for i in $(SUB_DIRS); do \
		make -C $$i --no-print-directory; \
	done

main:
	@make -C Source --no-print-directory source_server

clean_all::
	@${RM} $(NAME).o $(NAME) depends
	@for i in $(SUB_DIRS); do \
		echo "cleaning $$i"; \
		make -C $$i --no-print-directory clean; \
	done
