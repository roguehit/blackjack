CC=gcc
SRC=blackapple.c
CFLAGS=-Wall -g

apple:  $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o apple 

.phony: clean

clean:
	@rm apple 
	@echo Cleaned
