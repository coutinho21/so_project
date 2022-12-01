#clean and recompile the Q1 project
all: clean samples

#compile samples.c
samples: samples.c
	$(CC) $(CFLAGS) -o samples samples.c

#clean executables
clean:
	rm -f samples.o samples
