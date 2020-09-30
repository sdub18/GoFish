//
//  gofish.h
//  322-Assignment2
//
//  Created by Sam DuBois on 9/29/20.
//

#ifndef gofish_h
#define gofish_h

#include <stdio.h>
#include "player.h"
#include "deck.h"
#include "card.h"

// Function - Prints out the values of the hand of the target player
void printHand(player *target);

// Function - Prints out the values of the book of the target player
void printBook(player *target);

// Function - Prints out the values of a hand with a specific rank from a target player
void printHandWithRank(player *target, char rank);

// Function - Prints out the total numerical amount of books that a player possesses
int getSizeOfBook(player *target);

#endif /* gofish_h */
