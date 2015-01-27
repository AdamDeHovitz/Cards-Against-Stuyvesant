#Cards-Against-Stuyvesant
###Adam DeHovitz, Derek Tsui, Jason Lee
####Period 8

##Compilation Instructions

* compile with make

##Usage Instructions

* run ./server
 * specify the number of users
* run ./client in as many other terminals as specified in first step
 * follow instructions given
 

##Bugs
* If you don't give it the correct input (for example giving a garbage string instead of number when choosing a card) then it will not work
 * Much like the developers of C, we don't care about your experience. Your fault for following instructions
* Some black cards have two blanks. This was going to be implemented, but time has failed us
* Cards have been screened to make sure they are appropriate, but I'm only human. If something inappropriate sneaks through I am very sorry
* If you're the judge and you keyboard interrupt, the server doesn't run terminate() (int was a barrier)
* If you keyboard interrupt before all the users have joined you'll get a seg fault before quiting. (an additional global int would fix this)


##List of Files
* makefile
* deck.h
* deck.c
* server.c
* client.h
* client.c
* blackcards.txt
* whitecards.txt
* some notes/updates that are not necessary

##Original repository 
* https://github.com/AdamDeHovitz/Cards-Against-Stuyvesant

