#ifndef BLACKJACK_H
#define BLACKJACK_H

#include<stdio.h>
#include<ctype.h>
#include<assert.h> 
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#define INITIAL_CASH 500 
#define BLACKJACK 666
#define ROUND_COMPLETE 4
#define ROUND_IN_PROGRESS 2

/*Number of Rounds*/
int rounds=0;
/*Array to keep track of cards drawn*/
int track_card[52];

int cash_on_table;

/*The following structure code is from 
http://okmij.org/ftp/packages/black-jack.c
Only the Structure Card is copied, nothing else :p
*/

struct Card {
	enum Suit { Spades, Clubs, Hearts, Diamonds,Joker } suit;
	int value;
	char * const name;
} Deck [53] =
{
	{Joker,  0, "Joker"},
	{Spades, 2, "2"},
	{Spades, 3, "3"},
	{Spades, 4, "4"},
	{Spades, 5, "5"},
	{Spades, 6, "6"},
	{Spades, 7, "7"},
	{Spades, 8, "8"},
	{Spades, 9, "9"},
	{Spades, 10, "10"},
	{Spades, 10, "Jack"},
	{Spades, 10, "Queen"},
	{Spades, 10, "King"},
	{Spades, 11, "Ace"},

	{Clubs, 2, "2"},
	{Clubs, 3, "3"},
	{Clubs, 4, "4"},
	{Clubs, 5, "5"},
	{Clubs, 6, "6"},
	{Clubs, 7, "7"},
	{Clubs, 8, "8"},
	{Clubs, 9, "9"},
	{Clubs, 10, "10"},
	{Clubs, 10, "Jack"},
	{Clubs, 10, "Queen"},
	{Clubs, 10, "King"},
	{Clubs, 11, "Ace"},

	{Hearts, 2, "2"},
	{Hearts, 3, "3"},
	{Hearts, 4, "4"},
	{Hearts, 5, "5"},
	{Hearts, 6, "6"},
	{Hearts, 7, "7"},
	{Hearts, 8, "8"},
	{Hearts, 9, "9"},
	{Hearts, 10, "10"},
	{Hearts, 10, "Jack"},
	{Hearts, 10, "Queen"},
	{Hearts, 10, "King"},
	{Hearts, 11, "Ace"},


	{Diamonds, 2, "2"},
	{Diamonds, 3, "3"},
	{Diamonds, 4, "4"},
	{Diamonds, 5, "5"},
	{Diamonds, 6, "6"},
	{Diamonds, 7, "7"},
	{Diamonds, 8, "8"},
	{Diamonds, 9, "9"},
	{Diamonds, 10, "10"},
	{Diamonds, 10, "Jack"},
	{Diamonds, 10, "Queen"},
	{Diamonds, 10, "King"},
	{Diamonds, 11, "Ace"},
};
/*
Structure to keep track of player & dealer
*/
struct _player
{
	int wins;
	int cash;
	int hard_total;
	int soft_total;
	int cards_in_hand[10];
};

typedef struct _player player;

#endif
