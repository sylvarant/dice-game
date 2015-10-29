#########################################
#     dobbelen Makefile                    
# By Adriaan Larmuseau                  
#########################################

# program name
NAME= dobbelen

# to be linked objects
CSOURCES= util.o scorecard.o player.o dobbelen.o 

# compile with all warnings
CWARNFLAGS= -Wall -Wextra -std=gnu99 -Wno-variadic-macros \
		-Wno-unused-parameter -Wcast-align -Wwrite-strings \
		-Wmissing-prototypes -Wmissing-declarations \
		-Wredundant-decls -Wstrict-prototypes -Wshadow -Wcast-qual

CFLAGS= $(CWARNFLAGS) -g -O2 -lm -pg

all: $(CSOURCES) link

clean:
	$(RM) *.o $(NAME) LOG* *.tar gmon.* callgrind.out.* *.zip

link:
	gcc  -o $(NAME) $(CSOURCES) -pg -lm

	@echo $(NAME) succesfully built !!

tarball:
	tar -cf $(NAME).tar *.c *.h Makefile 

zipfile:
	zip $(NAME).zip *.c *.h Makefile

