/*
A simple blackjack game.
*/

#include "blackjack.h"

/*Shuffling Function
The code does not shuffle cards initially
It picks a random card while dealing
So the shuffling function just puts all cards 
back in the deck
*/
void shuffle_card()
{
	int i=0;
	for(i=0;i<52;i++)
		track_card[i]=0;
}


/*
Randomly pick a card from deck
This uses recursion to pick the card. 
In case Card is already dealt then call it 
again with an increased index
*/
int draw_card(int index)
{

	assert(index>0 && index<=52);
	int i=0;
	for(i=0;i<52;i++){
		if(!track_card[index]){
			track_card[index]=1;
			return (index);
		}
		else break;
	}
#ifdef DEBUG
	printf("Card %d is already dealt\n",index);
#endif
	draw_card( index%52+1 );

	return index;
}

/*
Print the cards for the player
Limit argument is the limit on number 
of cards to be displayed
*/
void print_handcard(player *p,int limit)
{
	int i=0;
#ifdef DEBUG
	//printf("%d index:%d\n",Deck[index].suit,index);
#endif

	if(p->cash==-1)
		printf("\nDealer has");
	else
		printf("\nYou have");

	for(i=0;i<limit;i++)
	{
		int index=p->cards_in_hand[i];
		switch(Deck[index].suit)
		{
			case 0:
				printf(" %s of %s ",Deck[index].name,"Spades");
				break;
			case 1:
				printf(" %s of %s ",Deck[index].name,"Clubs");
				break;
			case 2:
				printf(" %s of %s ",Deck[index].name,"Hearts");
				break;
			case 3:
				printf(" %s of %s ",Deck[index].name,"Diamonds");
				break;
			default:
				assert(0);
		}
		if(i< (limit-1))
			printf("&");
	}

	if(p->cash!=-1){
		if(p->hard_total==p->soft_total)
			printf(" (%d) \n",p->hard_total);
		else
			printf(" (%d/%d) \n",p->hard_total,p->soft_total);
		return;
	}
	for(i=0;p->cards_in_hand[i]!=0;i++);
	if(limit==i){
		if(p->hard_total==p->soft_total)
			printf(" (%d) \n",p->hard_total);
		else
			printf(" (%d/%d) \n",p->hard_total,p->soft_total);
	}
	return;
}

/*
Initialization Function
*/
int init (player **p)
{
	*p=(player*)malloc(sizeof(player));
	if(*p==NULL){
		fprintf(stderr,"Malloc Failed\n");
		return -1;
	}

	if(memset(*p,0,sizeof(player)*sizeof(char))==NULL)
		return -1;
	else
		return  0;
}

/*Function to take care of quirkyness of scanf
Code from - http://www.drpaulcarter.com/cs/common-c-errors.php 
Section 4.3 
*/
void flush_scanf()
{
	int c;
	while( (c=getc(stdin))!=EOF && c!='\n');
}

/*
Present initial options
*/
char present_options()
{
	char choice=0;
	system("clear");
	printf("------------Welcome to Blackjack-------------\n\n");
	printf("           Play (p) or Quit (q)\n");
	scanf("%c",&choice);
	while(choice!='p' && choice!='P' && choice!='q' && choice!='Q')
	{
		printf("Please make the right selection\n");
		flush_scanf();
		scanf("%c",&choice);
	}
	return choice;
}

/*
Cleanup after every round
*/
void round_cleanup(player **user, player** dealer)
{
	int i=0;
	(*user)->soft_total=(*user)->hard_total=0;
	(*dealer)->soft_total=(*dealer)->hard_total=0;

	for(i=0;i<10;i++){
		(*user)->cards_in_hand[i]=0;
		(*dealer)->cards_in_hand[i]=0;
	}
	return;
}
/*
Function that updates stats of the winner
*/
void won(player** p)
{
	if((*p)->cash==-1){
		printf("\nYou lost :( Dealer Blackjacked you\n");
	}
	else{
		printf("\nBlackjack! You Won the round !\n");
		(*p)->cash+=cash_on_table;
	}
	(*p)->wins++;
	cash_on_table=0;
	return;
}

/*
Function checks for a blackjack i.e. hard total = 21
*/
int check_blackjack(player **user, player **dealer)
{
	//assert((*user)->hard_total!=21   &&   (*dealer)->hard_total!=21);
	if((*user)->hard_total==21){
		won(user);
		return ROUND_COMPLETE;
	}else if((*user)->hard_total>21  &&  (*user)->soft_total>21 ){
		printf("\nBusted :(\n");
		won(dealer);
		return ROUND_COMPLETE;
	}

	if(  (*dealer)->hard_total==21){
		won(dealer);
		return ROUND_COMPLETE;
	}else if((*dealer)->hard_total>21 && (*dealer)->soft_total>21 ){
		printf("\nDealer Busted :)\n");
		won(user);
		return ROUND_COMPLETE;
	}

	return ROUND_IN_PROGRESS;
}

/*
Update the total for the player
*/
void update_total(player **p,int pull)
{
	int i=0;
	if(!strcmp(Deck[pull].name,"Ace"))
	{ (*p)->hard_total+=11;(*p)->soft_total+=1;}
	else{
		(*p)->hard_total+=Deck[pull].value;
		(*p)->soft_total+=Deck[pull].value;
	}

	for(i=0;(*p)->cards_in_hand[i]!=0;i++);
	(*p)->cards_in_hand[i]=pull;
	return;
}

/*
Function for a new round
*/
int new_round(player **user, player** dealer)
{
	int i,pull;

	srand(time(0));

	for(i=0;i<2;i++)
	{
		pull=draw_card(rand()%52+1);
		update_total(user,pull);

		pull=draw_card(rand()%52+1);
		update_total(dealer,pull);
	}

	print_handcard(*dealer,2);
	print_handcard(*user,2);

	return check_blackjack(user,dealer);
}

/*
Function to present midgame options 
*/
char present_midgame_options()
{
	char choice;
	printf("\n	Hit (h) Stand(s) Surrender(x) Exit-game(e)\n");
	flush_scanf();
	scanf("%c",&choice);
	while(choice!='h' && choice!='H' && choice!='s' && choice!='S' && choice!='x' && choice !='X' && choice !='e' && choice !='E')
	{
		printf("	Please make the right selection\n");
		flush_scanf();
		scanf("%c",&choice);
	}
	return choice;

}

/*
Function to bet money
*/
void bet_cash (player *user)
{
	assert(user->cash > 0);
	int bet;
	printf("Bet an amount, you have $%d\n",user->cash);
	flush_scanf();
	scanf("%d",&bet);
	while (bet > user->cash){ 
		printf("You Dont have that much money, Bet a smaller amount\n");
		flush_scanf();
		scanf("%d",&bet);
	}
	cash_on_table+=bet*2;
	user->cash-=bet;
	assert(user->cash >= 0);
	return;
}

/*
Function when user hits "hit"
*/
int hit(player **user,player **dealer)
{
	int i,pull;
	pull=draw_card(rand()%52+1);
	update_total(user,pull);
	for(i=0;(*user)->cards_in_hand[i]!=0;i++);
	print_handcard(*user,i);
	return check_blackjack(user,dealer);
}

/*
Function to make decision for dealer
Implements a simple AI, probably too simple :p
*/
int make_decision(player **user, player **dealer)
{
	int i,pull;

	/* Simple AI 
	   Always draw if total is < 17
	   if total > 17 , draw on the basis of
	   binary return value of rand()>>30
	 */

	if( ((*dealer)->hard_total < 17) || rand()>>30  ){
		pull=draw_card(rand()%52+1);
		update_total(dealer,pull);
		for(i=0;(*user)->cards_in_hand[i]!=0;i++);
		print_handcard(*dealer,i);
		return check_blackjack(user,dealer);
	}
	else return ROUND_COMPLETE;
}

/*
Called when user hits stand "s"
*/
int stand(player **user, player **dealer)
{

	int pull,i,ret;
	/*Dealer plays till Round is complete*/

	while(1){
		for(i=0;(*dealer)->cards_in_hand[i]!=0;i++);

		if((*dealer)->soft_total == (*dealer)->hard_total)
		{/*Code for hardness*/
			if((*user)->soft_total == (*user)->hard_total)
			{
				if( ((*user)->hard_total >= (*dealer)->hard_total) )
				{
					ret = make_decision(user,dealer);				 
				}
				else
				{
					won(dealer);
					ret = ROUND_COMPLETE;
				}
			}
			else
			{
				if((*user)->hard_total > 21)		
				{
					if((*user)->soft_total >= (*dealer)->hard_total)
					{
						ret = make_decision(user,dealer);
					}
					else
					{
						won(dealer);
						ret = ROUND_COMPLETE;
					}
				}
				else
				{
					if((*user)->hard_total >= (*dealer)->hard_total)
					{
						ret = make_decision(user,dealer);
					}
					else
					{
						won(dealer);
						ret = ROUND_COMPLETE;
					}

				}

			}

		}
		else
		{
			/*Soft 17 Condition*/
			if( ((*dealer)->soft_total == 6 ) &&  ((*dealer)->hard_total==17) )
			{
				pull=draw_card(rand()%52+1);
				update_total(dealer,pull);

				for(i=0;(*user)->cards_in_hand[i]!=0;i++);
				print_handcard(*dealer,i);
				ret=check_blackjack(user,dealer);
			}

			else if ((*dealer)->hard_total > 21)
			{

				if((*user)->hard_total > 21)		
				{
					if((*user)->soft_total >= (*dealer)->soft_total)
					{
						ret = make_decision(user,dealer);
					}
					else
					{
						won(dealer);
						ret = ROUND_COMPLETE;
					}
				}
				else
				{
					if((*user)->hard_total >= (*dealer)->soft_total)
					{
						ret = make_decision(user,dealer);
					}
					else
					{
						won(dealer);
						ret = ROUND_COMPLETE;
					}

				}

			} 
			else
			{

				if((*user)->hard_total > 21)		
				{
					if((*user)->soft_total >= (*dealer)->hard_total)
					{
						ret = make_decision(user,dealer);
					}
					else
					{
						won(dealer);
						ret = ROUND_COMPLETE;
					}
				}
				else
				{
					if((*user)->hard_total >= (*dealer)->hard_total)
					{
						ret = make_decision(user,dealer);
					}
					else
					{
						won(dealer);
						ret = ROUND_COMPLETE;
					}

				}

			}

		}

		if (ret == ROUND_COMPLETE)
			return ROUND_COMPLETE;

	}

}

void print_stats(player **user)
{
	printf("\nWin Percentage = %3.2f %% \n",(float)(*user)->wins/rounds*100);
	return;
}

/*
Main play funtion, called from main
*/
void play(player **user, player** dealer)
{
	system("clear");
	int proceed=ROUND_IN_PROGRESS;
	char choice;
	printf("Lets play\n");
	while((*user)->cash>0){
		bet_cash(*user);

		if(new_round(user,dealer)==ROUND_IN_PROGRESS)
		{
			while(proceed==ROUND_IN_PROGRESS)
			{
				choice=present_midgame_options();

				switch(choice)
				{
					case 'h':
					case 'H':
						proceed=hit(user,dealer);
						break;
					case 's':
					case 'S':
						proceed=stand(user,dealer);
						break;
					case 'x':
					case 'X':
						proceed=ROUND_COMPLETE;
						break;
					case 'e':
					case 'E':
						print_stats(user);
						exit(0);
					default:
						assert(0);
				}
			}
			round_cleanup(user,dealer);
			proceed=ROUND_IN_PROGRESS;
		}
		round_cleanup(user,dealer);
		rounds++;
		if(rounds % 6 == 0 )
			shuffle_card();
		print_stats(user);
	}
	printf("Out of Cash\n");
	sleep(3);
}

int main(int argc, char**argv)
{

	player *user=NULL, *dealer=NULL;
	char choice;

	if(init(&user)!=0){
		fprintf(stderr,"Could Not Create Player\n");
		exit(0);
	}

	if(init(&dealer)!=0){
		fprintf(stderr,"Could Not Create Dealer\n");
		exit(0);
	}

	user->cash=500;
	dealer->cash=-1;
	shuffle_card();

	choice=present_options();

	switch(choice)
	{
		case 'p':
		case 'P':
			play(&user,&dealer);
			break;
		case 'q':
		case 'Q':
			printf("....Quitting\n");
			break;
	}


	return 0;
}

