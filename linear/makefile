PRG            = $(shell basename $(PWD))
OBJ            = $(PRG).o
OPTIMIZE       = -O2
DEFS           =
LIBS           =
# You should not have to change anything below here.
CC             = gcc
# Override is only needed by avr-lib build system.
override CFLAGS        = -Wall $(OPTIMIZE) $(DEFS)
all: $(PRG)
$(PRG): *.o
	$(CC) $(CFLAGS) *.o -o $@ -lm
# dependency:
*.o: *.c
	$(CC) $(CFLAGS) -c $^
clean:
	rm -rf *.o *.gch *.csv 
