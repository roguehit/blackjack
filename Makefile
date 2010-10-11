CC=gcc
SRC=blackjack.c
CFLAGS=-Wall -g

blackjack:  $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o blackjack

.phony: clean

clean:
	@rm blackjack 
	@echo Cleaned
