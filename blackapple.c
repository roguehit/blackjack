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

int rounds=0;


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

struct _player
{
int wins;
int cash;
int hard_total;
int soft_total;
int cards_in_hand[10];
};

typedef struct _player player;

int track_card[52];
int cash_on_table;

void shuffle_card()
{
int i=0;
for(i=0;i<52;i++)
track_card[i]=0;
}


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


}

void print_handcard(player *p,int limit)
{
int i=0;
#ifdef DEBUG
printf("%d index:%d\n",Deck[index].suit,index);
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

void flush_scanf()
{
int c;
while( (c=getc(stdin))!=EOF && c!='\n');
}

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

void won(player** p)
{
if((*p)->cash==-1){
printf("\nYou lost :( Dealer Blackjacked you\n");
(*p)->wins++;
}
else
printf("\nBlackjack! You Won the round !\n");
(*p)->wins++;
(*p)->cash+=cash_on_table;
cash_on_table=0;
return;
}

int check_blackjack(player **user, player **dealer)
{
//assert((*user)->hard_total!=21   &&   (*dealer)->hard_total!=21);
if((*user)->hard_total==21){
won(user);
return ROUND_COMPLETE;
}else if((*user)->hard_total>21  &&  (*user)->soft_total>21 ){
printf("\nBusted :(\n");
return ROUND_COMPLETE;
}

if(  (*dealer)->hard_total==21){
won(dealer);
return ROUND_COMPLETE;
}else if((*dealer)->hard_total>21 && (*dealer)->soft_total>21 ){
printf("\nDealer Busted :)\n");
return ROUND_COMPLETE;
}

return ROUND_IN_PROGRESS;
}

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

print_handcard(*dealer,1);
print_handcard(*user,2);

return check_blackjack(user,dealer);
}

char present_midgame_options()
{
char choice;
printf("\n	Hit (h) Stand(s) Surrender(x)\n");
flush_scanf();
scanf("%c",&choice);
while(choice!='h' && choice!='H' && choice!='s' && choice!='S' && choice!='x' && choice !='X')
{
printf("	Please make the right selection\n");
flush_scanf();
scanf("%c",&choice);
}
return choice;

}
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

int hit(player **user,player **dealer)
{
int i,pull;
pull=draw_card(rand()%52+1);
update_total(user,pull);
for(i=0;(*user)->cards_in_hand[i]!=0;i++);
print_handcard(*user,i);
return check_blackjack(user,dealer);
}

int stand(player **user, player **dealer)
{

int pull,i;
for(i=0;(*dealer)->cards_in_hand[i]!=0;i++);

if((*dealer)->soft_total==17 || (*dealer)->hard_total<10 ){
pull=draw_card(rand()%52+1);
}
else
/*Here comes the main AI*/
pull=draw_card(rand()%52+1);


update_total(dealer,pull);
print_handcard(*dealer,i+1);

if(check_blackjack(user,dealer)==ROUND_IN_PROGRESS){
if((*user)->soft_total==(*user)->hard_total && (*dealer)->soft_total==(*dealer)->hard_total ){
if((*user)->hard_total > (*dealer)->hard_total)
won(user);
else 
won(dealer);
}
}
return ROUND_COMPLETE;
}



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
//surrender(user,dealer);
printf("Here comes code to Surrender\n");
break;
default:
assert(0);
}
}
round_cleanup(user,dealer);
proceed=ROUND_IN_PROGRESS;
}
round_cleanup(user,dealer);
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
 
