#clean and recompile the Q2 project
all: clean text2epub

#compile text2epub.c
text2epub: text2epub.c
	$(CC) $(CFLAGS) -o text2epub text2epub.c

#clean pipes
clean:
	rm -f text2epub.o text2epub *.epub *.zip
