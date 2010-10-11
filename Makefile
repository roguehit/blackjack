CC=gcc
SRC=blackjack.c
CFLAGS=-Wall -g

apple:  $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o blackjack

.phony: clean

clean:
	@rm blackjack 
	@echo Cleaned
