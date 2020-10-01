//
//  player.c
//  322-Assignment2
//
//  Created by Sam DuBois on 9/29/20.
//

#include "player.h"

/*
 * Function: add_card
 * -------------------
 *  Add a new card to the player's hand.
 *
 *  target: the target player
 *  new_card: pointer to the new card to add
 *
 *  returns: return 0 if no error, non-zero otherwise
 */
int add_card(struct player* target, struct card* new_card) {
    hand *currHand = target->card_list;
    
    // Instantiate new hand with value
    hand *newHand = (hand *) malloc(sizeof(hand));
    
    if (newHand == NULL) {
        printf("Not enough memory to allocate item");
        return 1;
    }
    
    newHand->top = *new_card;
    newHand->next = NULL;
    
    // Check to see if card list is empty or not
    if (currHand == NULL) {
        target->card_list = newHand;
    } else {
        // Get to the end of of the card list
        while (currHand->next != NULL) {
            currHand = currHand->next;
        }
        
        currHand->next = newHand;
    }
    
    target->hand_size ++;
    return 0;
}

/*
 * Function: remove_card
 * ---------------------
 *  Remove a card from the player's hand.
 *
 *  target: the target player
 *  old_card: pointer to the old card to remove
 *
 *  returns: return 0 if no error, non-zero otherwise
 */
int remove_card(struct player* target, struct card* old_card) {
    hand *currHand = target->card_list;
    
    // Check first card
    if (currHand->top.rank[1] == old_card->rank[1] && currHand->top.suit == old_card->suit) {
        hand *link = currHand->next;
        target->card_list = link;
        target->hand_size --;
        return 0;
    }
    
    while(currHand->next != NULL) {
        if (currHand->next->top.rank[1] == old_card->rank[1] && currHand->next->top.suit == old_card->suit) {
            hand *link = currHand->next->next;
            currHand->next = link;
            target->hand_size --;
            return 0;
        }
        currHand = currHand->next;
    }
    return 1;
}

/*
 * Function: check_add_book
 * ------------------------
 *  Check if a player has all 4 cards of the same rank.
 *  If so, remove those cards from the hand, and add the rank to the book.
 *  Returns after finding one matching set of 4, so should be called after adding each a new card.
 *
 *  target: pointer to the player to check
 *
 *  Return: a char that indicates the book that was added; return 0 if no book added.
 */
char check_add_book(struct player* target) {
    
    // Get the last card added to the player's card_list
    hand *lastHand = target->card_list;
    
    while(lastHand->next != NULL) {
        lastHand = lastHand->next;
    }
    
    // See if there are more of this card
    hand *matches[4];
    hand * currHand = target->card_list;
    int index = 0;
    int tally = 0;
    
    while (currHand != NULL) {
        if (currHand->top.rank[1] == lastHand->top.rank[1]) {
            matches[index] = currHand;
            index ++;
            tally ++;
        }
        currHand = currHand->next;
    }
    
    // If we found 4 matches
    if (tally == 4) {
        
        // Record card rank
        char rank = lastHand->top.rank[1];
        
        // Add it to the book
        for (int i=0; i<7; i++) {
            if (target->book[i] == 0) {
                target->book[i] = rank;
                break;
            }
        }
        
        for(int i=0; i<4; i++) {
            remove_card(target, &matches[i]->top);
        }
        
        return rank;
    }
    
    return 0;
}

/*
 * Function: search
 * ----------------
 *  Search a player's hand for a requested rank.
 *
 *  rank: the rank to search for
 *  target: the player (and their hand) to search
 *
 *  Return: If the player has a card of that rank, return 1, else return 0
 */
int search(struct player* target, char rank) {
    
    hand *currHand = target->card_list;
    
    while (currHand != NULL) {
        if (currHand->top.rank[1] == rank) {
            return 1;
        }
        currHand = currHand->next;
    }
    
    return 0;
}

/*
 * Function: transfer_cards
 * ------------------------
 *   Transfer cards of a given rank from the source player's
 *   hand to the destination player's hand. Remove transferred
 *   cards from the source player's hand. Add transferred cards
 *   to the destination player's hand.
 *
 *   src: a pointer to the source player
 *   dest: a pointer to the destination player
 *   rank: the rank to transfer
 *
 *   Return: 0 if no cards found/transferred, <0 if error, otherwise
 *   return value indicates number of cards transferred
 */
int transfer_cards(struct player* src, struct player* dest, char rank) {
    
    // get end of destination hand
    hand *lastDestHand = dest->card_list;
    
    while (lastDestHand->next != NULL) {
        lastDestHand = lastDestHand->next;
    }
    
    // Cycle through number of cards of source
    hand* currSrcHand = src->card_list;
    int tally = 0;
    
    while (currSrcHand!= NULL) {
        char tempRank = currSrcHand->top.rank[1];
        if ( tempRank == rank) {
            remove_card(src, &currSrcHand->top);
            add_card(dest, &currSrcHand->top);
            tally ++;
        }
        currSrcHand = currSrcHand->next;
    }
    
    return tally;
}

/*
 * Function: game_over
 * -------------------
 *   Boolean function to check if a player has 7 books yet
 *   and the game is over
 *
 *   target: the player to check
 *
 *   Return: 1 if game is over, 0 if game is not over
 */
int game_over(struct player* target) {
    
    if (target->book[6] == 0) {
        return 0;
    }
    return 1;
}

/*
 * Function: reset_player
 * ----------------------
 *
 *   Reset player by free'ing any memory of cards remaining in hand,
 *   and re-initializes the book.  Used when playing a new game.
 *
 *   target: player to reset
 *
 *   Return: 0 if no error, and non-zero on error.
 */
int reset_player(struct player* target) {
    
    hand *currHand = target->card_list;
    
    while (currHand != NULL) {
        hand *tmp = currHand;
        currHand = currHand->next;
        free(tmp);
    }
    
    target->card_list = NULL;
    
    for (int i=0; i<7; i++) {
        target->book[i] = 0;
    }
    
    target->hand_size = 0;
    
    return 0;
}

/*
 * Function: computer_play
 * -----------------------
 *
 *   Select a rank randomly to play this turn. The player must have at least
 *   one card of the selected rank in their hand.
 *
 *   target: the player's hand to select from
 *
 *   Rank: return a valid selected rank
 */
char computer_play(struct player* target) {
    
    // Get Current hand and a random number between 0 and hand size
    hand *currHand = target->card_list;
    int handNum = (rand() % (target->hand_size + 1));
    
    // go to that hand
    for(int i=0; i<handNum; i++) {
        currHand = currHand->next;
    }
    
    printf( "Player 2's turn, enter a Rank:");
    if(currHand==NULL){
        currHand=target->card_list;
    }
    
    // get the rank from that hand
    char rank = currHand->top.rank[1];
    
    if (rank == '1') {
        printf("10\n");
    } else {
        printf("%c\n", rank);
    }
    
    return rank;
}

/*
 * Function: user_play
 * -------------------
 *
 *   Read standard input to get rank user wishes to play.  Must perform error
 *   checking to make sure at least one card in the player's hand is of the
 *   requested rank.  If not, print out "Error - must have at least one card from rank to play"
 *   and then re-prompt the user.
 *
 *   target: the player's hand to check
 *
 *   returns: return a valid selected rank
 */
char user_play(struct player* target) {
    
    char userInput[100];
    int didEnterValidInput = 0;
    
    char selectedRank;
    
    // First get requested card type
    while (didEnterValidInput == 0) {
        printf( "Player 1's turn, enter a Rank:");
        int validScan = scanf("%s", userInput);
        printf("\n");
        
        if (validScan != 1) {
            printf("Error - must have at least one card from rank to play \n");
            continue;
        }
        
        selectedRank = userInput[0];
        
        if (search(target, selectedRank) == 0) {
            printf("Error - must have at least one card from rank to play \n");
            continue;
        } else {
            didEnterValidInput = 1;
            return selectedRank;
        }
    };
    return 0;
}
