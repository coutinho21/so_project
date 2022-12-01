#clean and recompile the Q3 project
all: clean tokenring

#compile tokenring.c
tokenring: tokenring.c
	$(CC) $(CFLAGS) -o tokenring tokenring.c

#clean pipes and executables
clean:
	rm -f pipe* tokenring.o tokenring
