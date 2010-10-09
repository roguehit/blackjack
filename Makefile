CC=gcc
SRC=blackapple.c 
DEBUG=-DDEBUG
CFLAGS=-Wall -g

apple: blackapple.c
	$(CC) $(SRC) $(CFLAGS) -o apple 

.phony: clean debug

debug:
	$(CC) $(SRC)  $(CFLAGS) $(DEBUG) -o apple 
clean:
	@rm apple 
	@echo Cleaned
