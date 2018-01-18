PRG            = $(shell basename $(PWD))
OBJ            = $(PRG).o
OPTIMIZE       = -O2
DEFS           =
LIBS           =
# You should not have to change anything below here.
CC             = g++
# Override is only needed by avr-lib build system.
override CFLAGS        = -Wall $(OPTIMIZE) $(DEFS)
all: $(PRG)
$(PRG): *.c
	$(CC) $(CFLAGS) $^ -o $@ -lm -lpthread -fopenmp
# dependency:
clean:
	rm -rf *.o *.gch $(PRG) *.csv
