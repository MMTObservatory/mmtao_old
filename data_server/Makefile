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

NAME = Data_Server

#
# List of subdirectories containing needed libraries (routines)
#

SUB_DIRS = \
	Error \
	DServ \
	Info \
	PCR \
	Socket \
	Source \
	Status \
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
include ./include/PCR_Make_Flags
#
# Compile and link main routine
#
$(NAME).x: subdirectories main $(NAME).o
	@echo ' '
	@echo '   linking '$(NAME)''
	@echo ' '
	@$(LINKER) $(CFLAGS) -o data_server \
		$(NAME).o \
		$(LINK_LIST) \
		$(LD_FLAGS)
#
$(NAME).o: $(NAME).c $(ADD_INS)
	$(CC) $(CFLAGS) -c $(NAME).c

#
# Compile and archive functions
#
include ./include/PCR_Make_Dependencies
#

subdirectories:
	@for i in $(SUB_DIRS); do \
		make -C $$i --no-print-directory; \
	done

main:
	@make -C Source --no-print-directory cmd
	@make -C Source --no-print-directory dm
	@make -C Source --no-print-directory wfsc
	@make -C Source --no-print-directory error

clean_all::
	@${RM} $(NAME).o $(NAME) depends
	@for i in $(SUB_DIRS); do \
		echo "cleaning $$i"; \
		make -C $$i --no-print-directory clean; \
	done
	@cd Source; rm -f cmd
	@cd Source; rm -f dm
	@cd Source; rm -f wfsc
	@cd Source; rm -f error
