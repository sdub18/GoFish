//
//  deck.c
//  322-Assignment2
//
//  Created by Sam DuBois on 9/29/20.
//

#include "deck.h"


extern struct deck deck_instance;

/*
 * Function: shuffle
 * --------------------
 *  Initializes deck_instance and shuffles it.
 *  Resets the deck if a prior game has been played.
 *
 *  returns: 0 if no error, and non-zero on error
 */
int shuffle(){
    char suitArr[4] = {'C','H','D','S'};
    char rankArr[13] = {'A','2','3','4','5','6','7','8','9','1','J','Q','K'};
    int counter = 0;
    // creates deck
    for(int i=0; i<4; i++){
        for(int j=0; j<13; j++){
            deck_instance.list[counter].suit = suitArr[i];
            deck_instance.list[counter].rank[1] = rankArr[j];
            counter++;
        }
    }
    //shuffle deck
    for(int k=0; k<52; k++){
        size_t l = k + rand() / (RAND_MAX/(52-k)+1);
        struct card t = deck_instance.list[l];
        deck_instance.list[l] = deck_instance.list[k];
        deck_instance.list[k] = t;
    }
    deck_instance.top_card = 51;
    return 0;
}


/*
 * Function: deal_player_cards
 * ---------------------------
 *  Deal 7 random cards to the player specified in the function.
 *  Remove the dealt cards from the deck.
 *
 *  target: pointer to the player to be dealt cards
 *
 *  returns: 0 if no error, and non-zero on error
 */
int deal_player_cards(struct player* target){
    int counter1=0;
    int counter2= (deck_instance.top_card == 0) ? 51 : deck_instance.top_card;
    struct hand* tempHand;
    struct card* tempCard;
    tempHand = target->card_list;
    while(counter1<7){
        if(deck_instance.list[counter2].rank[1] == 0){
            counter2--;
        }
        else{
            tempCard = &deck_instance.list[counter2];
            add_card(target,tempCard);
            
            // Replace instance with a zero:
            deck_instance.list[counter2].rank[1] = 0;
            
            counter1++;
            counter2 --;
        }
    }
    
    deck_instance.top_card = counter2;
    
    return 0;
}

/*
 * Function: next_card
 * -------------------
 *  Return a pointer to the top card on the deck.
 *  Removes that card from the deck.
 *
 *  returns: pointer to the top card on the deck.
 */
struct card* next_card(){
    // Crete new card pointer
    struct card* tempCard = (struct card*)malloc(sizeof(struct card));
    int topCard = deck_instance.top_card;
    tempCard = &deck_instance.list[topCard];
    deck_instance.top_card --;
    return tempCard;
}

/*
 * Function: size
 * --------------
 *  Return the number of cards left in the current deck.
 *
 *  returns: number of cards left in the deck.
 */
size_t deck_size(){
    return deck_instance.top_card + 1;
}
