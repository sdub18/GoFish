//
//  gofish.c
//  322-Assignment2
//
//  Created by Sam DuBois on 9/29/20.
//

#include "gofish.h"

int main(int args, char* argv[]) {
    // MARK: Pregame Initialization
    
    int gameIsOver = 0;     // Determines if the round is still active
    int userPlaying = 1;    // Determiens if the user is interested in playing after finishing a game
    
    
    // MARK: Program Running While Loop
    while (userPlaying == 1) {
        
        // Set User and Computer Parameteres to default
        reset_player(&user);
        reset_player(&computer);
        
        // Shuffle Cards
        printf("Shuffling deck... \n");
        shuffle();
        
        // Assign player cards
        deal_player_cards(&user);
        deal_player_cards(&computer);
        
        int currentPlayersTurn = 0;     // Details the current player's turn | 0 == User | 1 == Computer |
        
        // MARK: Game Running While Loop
        while (gameIsOver == 0) {
            
            // MARK: Pre-Move Decision Making
            
            int bookCreated = 0;
            
            // Print Player One's Hand:
            printf("\nPlayer 1's Hand - ");
            printHand(&user);
            
            // Print Player One's Books:
            printf("Player 1's Book - ");
            printBook(&user);
            
            // Print Player Two's Books:
            printf("Player 2's Book - ");
            printBook(&user);
            
            if (currentPlayersTurn == 0) {
                // MARK: Player One's Turn
                
                // get selected rank
                char rank = user_play(&user);
                
                // See if player 2 has it
                if (search(&computer, rank) == 1) {
                    printf("    -Player 2 has ");
                    printHandWithRank(&computer, rank);
                    printf("    -Player 1 has ");
                    printHandWithRank(&user, rank);
                    transfer_cards(&computer, &user, rank);
                } else {
                    if (rank == '1') {
                        printf("    -Player 2 has no 10's \n");
                    } else {
                        printf("    -Player 2 has no %c's \n", rank);
                    }
                    
                    // Draw a card for player
                    card *drawnCard = next_card();
                    if (drawnCard->rank[1] == '1') {
                        printf("    -Go Fish, Player 1 draws 10%c\n", drawnCard->suit);
                    } else {
                        printf("    -Go Fish, Player 1 draws %c%c\n", drawnCard->rank[1], drawnCard->suit);
                    }
                    add_card(&user, drawnCard);
                }
                
                // Check if book is possible:
                char book = check_add_book(&user);
                if (book != 0) {
                    if (book == 1) {
                        printf("    -Player 1 books 10\n");
                    } else {
                        printf("    -Player 1 books %c\n", book);
                    }
                    
                    // Keep turn
                    printf("Player 1 gets another turn\n");
                    bookCreated = 1;
                }
                
            } else {
                // MARK: Player Two's Turn
                
                // get selected rank
                char rank = computer_play(&computer);
                
                // See if player 2 has it
                if (search(&user, rank) == 1) {
                    printf("    -Player 1 has ");
                    printHandWithRank(&user, rank);
                    printf("    -Player 2 has ");
                    printHandWithRank(&computer, rank);
                    transfer_cards(&user, &computer, rank);
                } else {
                    if (rank == 1) {
                        printf("    -Player 1 has no 10's \n");
                    } else {
                        printf("    -Player 1 has no %c's \n", rank);
                    }
                    
                    // Draw a card for player
                    card *drawnCard = next_card();
                    printf("    -Go Fish, Player 2 draws a card\n");
                    add_card(&computer, drawnCard);
                }
                
                // Check if book can be formed
                char book = check_add_book(&computer);
                if (book != 0) {
                    if (book == 1) {
                        printf("    -Player 2 books 10\n");
                    } else {
                        printf("    -Player 2 books %c\n", book);
                    }
                    
                    // Keep turn
                    printf("Player 2 gets another turn\n");
                    bookCreated = 1;
                }
                
            }
            
            // MARK: End of Round Cleanup
            
            // Check if game is over
            if (game_over(&user) == 1) {
                printf("Player 1 Wins! ");
                printf("7-%i\n", getSizeOfBook(&computer));
                gameIsOver = 1;
            } else if (game_over(&computer)){
                printf("Player 2 Wins! ");
                printf("7-%i\n", getSizeOfBook(&user));
                gameIsOver = 1;
            }
            
            // Switch The Turns (No Book was created)
            if (gameIsOver != 1) {
                if (bookCreated == 0) {
                    if (currentPlayersTurn == 1) {
                        currentPlayersTurn = 0;
                    } else {
                        currentPlayersTurn = 1;
                    }
                }
            }
        }
        return 0;
    }
    
    // MARK: Game has ended
    // Determine if the user wants to exit the game
    
    int didEnterValidInput = 1;
    char userInput[100];
    char response;
    
    // Ensure a proper response from scanf
    while (didEnterValidInput == 0) {
        printf( "\nDo you want to play again [Y|N]:");
        int validScan = scanf("%s", userInput);
        printf("\n");
        
        if (validScan != 1) {
            printf("Error - must reply with either Y or N \n");
            continue;
        }
        
        response = userInput[0];
        
        if (response != 'Y' && response != 'N' && response != 'y' && response != 'n') {
            printf("Error - must reply with either Y or N \n");
            continue;
        } else {
            didEnterValidInput = 1;
            
            // handle response
            if (response == 'Y' || response == 'y') {
                // Nothing to do
                continue;
            } else {
                printf("Exiting. \n");
                userPlaying = 0;
            }
        }
    }
}

void printHand(player *target) {
    hand *currentHand = target->card_list;
    for (int i=0; i<user.hand_size; i++) {
        
        // Add Code here to support for 10 --- Change all [0] to [1]
        if (currentHand->top.rank[1] == '1') {
            printf("10");
        } else {
            printf("%c", currentHand->top.rank[1]);
        }
        
        printf("%c ", currentHand->top.suit);
        currentHand = currentHand->next;
    }
    printf("\n");
}

void printBook(player *target) {
    int index = 0;
    while (target->book[index] != 0) {
        if (target->book[index] == 1) {
            printf("10");
        } else {
            printf("%c", target->book[index]);
        }
        index ++;
    }
    printf("\n");
}

void printHandWithRank(player *target, char rank) {
    hand *currentHand = target->card_list;
    
    while (currentHand != NULL) {
        if (currentHand->top.rank[1] == rank) {
            if (currentHand->top.rank[1] == '1') {
                printf("10");
            } else {
                printf("%c", currentHand->top.rank[1]);
            }
            printf("%c, ", currentHand->top.suit);
        }
        currentHand = currentHand->next;
    }
    printf("\n");
}

int getSizeOfBook(player *target) {
    int index = 0;
    
    while (target->book[index] != 0) {
        index ++;
    }
    
    return index + 1;
}
