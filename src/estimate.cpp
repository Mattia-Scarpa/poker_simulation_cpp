/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
----------------------------- IMPORTING LIBRARIES -----------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/

// Importing Standard Libraries

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Importing Utilities Libraries

#include <algorithm>
#include <chrono>
#include <thread> 
#include <math.h>
#include <assert.h>

// Importing Custom Libraries

#include <poker.h>



/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
--------------------------- DEFINE SYMBOLS VALUES -----------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/


#define COMBINATIONS_1_CARDS 52                             // Total of 1 card combination (used in turn to complete the board for the player)
#define COMBINATIONS_2_CARDS 1326                           // Total of 2 cards combination (to estimate from river phase)
#define COMBINATIONS_3_CARDS 22100                          // Total of 3 cards combination (to estimate from turn phase)




/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-------------------------------- INITIALIZATION -------------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/

int nCards[3] = {2, 3};                                     // Initialize the number of cards according to the phase of the game
                                                            // the CURRENT_PHASE (defined below)


short combs1[COMBINATIONS_1_CARDS] =    {  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
                                        11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
                                        21, 22, 23, 24, 52, 26, 27, 28, 29, 30, 
                                        31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
                                        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 
                                        51, 52  };
short combs2[COMBINATIONS_2_CARDS][2] = {0};
short combs3[COMBINATIONS_3_CARDS][3] = {0};


uint64_t initTime, finalTime;

float totalWin(0.f), totalSimulation(0.f);

uint8_t N_PLAYER (0);                                       // number of player on the board
                                                            // TO BE IMPLEMENTED IN REAL TIME

uint8_t CURRENT_PHASE (2);                                  // Phase of the game represented by # of card to complete the board from the current state
                                                            // 0 - River    /   1 - Turn
                                                            // TO BE IMPLEMENTED IN REAL TIME

std::vector<poker::card> boardList;                         // Information of the current board cards seen
poker::hand handList;                                       // Information of the current player hand
                                                            // TO BE IMPLEMENTED IN REAL TIME


int main(int argc, char const *argv[]) {
    
    std::cout << "Loading combinations files...\n";

    std::ifstream combsList;                                // Initialize the input streaming

    for (auto i : nCards) {                                 // For each combinations list required for different phases
                                                            // At river phase ony 2 cards are remaining so it are required all 2 cards combination
                                                            // the same reasoning apply to turn with 3 cards and flop with 4 cards remaining

        std::string path = "../saves/comb"+std::to_string(i)+"c.txt";

        combsList.open(path);                               // Open the imput stream from the file in path

        if (!combsList) {                                   // Check if the file has been correctly opened
            std::cerr << "No file named " << path << " was found!" << "\n";
            exit(1);
        }
        float c;
        int idx;
        if (i == 2) {
            idx = 0;
            std::cout << "Loading: " << path << "...\n";   
            while (combsList >> c) {                                                        // Reading values
                combs2[idx/2][idx%2] = c;                                                   // Placing remapped card at corresponding position in container array
                idx++;                                                                      // next element
            }
            combsList.close();
        }        
        if (i == 3) {
            idx = 0;
            std::cout << "Loading: " << path << "...\n";   
            while (combsList >> c) {                                                        // Reading values
                combs3[idx/3][idx%3] = c; ;                                                 // Placing at corresponding position in container array
                idx++;                                                                      // next element
            }
            combsList.close();
        }
    }
    std::cout << "Combination lists loaded!\n";

    return 0;
}
