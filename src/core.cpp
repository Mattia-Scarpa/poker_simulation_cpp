
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
#include <signal.h>
#include <math.h>

// Importing Custom Libraries

#include "poker.h"




/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
--------------------------- DEFINE SYMBOLS VALUES -----------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/

#define nFlop 3
#define nTurn 1
#define nRiver 1
 
#define COMBS_2CARD 1326                // Total of 2 cards combination
#define COMBS_5CARD 2598960             // Total of 5 cards combination



/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-------------------------------- INITIALIZATION -------------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/

std::vector<poker::card> deck = poker::utility::generateDeck();         // Initialize deck of 52 cards

uint64_t searchTime(0), loadTime(0), initTime(0), finalTime(0);
int count(0), draw(0);
double GAMES_NUMBER(0);                                                     // Number of games to play in the sessions (each game execute nThread simulations)
int PLAYER_COUNT(0);                                                        // Number of players on the table
int nThreads(0);                                                            // Number of simulation per games
std::vector<std::string> GAME_PHASE = {"_blind_", "_flop_"};                // phase of the game saved


// Initialize the combination and results list path in the corresponding folder


// Initialize containers to store all the combination
//nc::NdArray<short> combs2 = nc::zeros<short>(COMBS_2CARD, 2);                                               // 2 cards combination (blind)
//nc::NdArray<short> combs5 = nc::zeros<short>(COMBS_5CARD, 5);                                               // 5 cards combination (flop)

short combs2[COMBS_2CARD][2] {0};
short combs5[COMBS_5CARD][5] {0};

// Initialize result container for Win [Tie [Lose]]
std::vector<std::vector<float>> oddsResultBlind(COMBS_2CARD, std::vector<float>(2, 0));                     // 2 cards result (blind)
std::vector<std::vector<float>> oddsResultFlop(COMBS_5CARD, std::vector<float>(2, 0));                      // 5 cards result (blind)

std::ifstream combFile;                                             // Initialize the files stream



/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
----------------------------------- FUNCTIONS ---------------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/

// Binary search for hands
// Return the index of the first array corresponding to the values of the second vector
int handBinarySearch(short array[][2], std::vector<short> &value, int low, int high) {  
    // Check constraints on lists to be evaluated
    //assert(array[0].size() == value.size() && "Array and values must have the same number of columns");

    int numCols = value.size();        // Iterator index limit: the evaluation is made for all the elements in the row


    while (low != high) {
        
        int mid = (low+high)/2;          

        bool found = true;
        for (size_t i(0); i < numCols; i++)
            if (array[mid][i] != value[i]) found = false;            // Control if the two rows are equal
        
        if(found) return mid;                                           // If row are equal at mid position then return the current index
        else {
            for (int i(0); i<numCols; i++) {
                if(array[mid][i] > value[i]) {high = mid; break;}    // If value row is smaller then array in mid check in the bot half

                if(array[mid][i] < value[i]) {low = mid; break;}     // Otherwise chel in the top half
            }
        }
    }
    return -1;                                                          // If the equality check always fail then return -1 as failure
}
int handBinarySearch(short array[][5], std::vector<short> &value, int low, int high) {  
    // Check constraints on lists to be evaluated
    //assert(array[0].size() == value.size() && "Array and values must have the same number of columns");

    int numCols = value.size();        // Iterator index limit: the evaluation is made for all the elements in the row


    while (low != high) {
        
        int mid = (low+high)/2;          

        bool found = true;
        for (size_t i(0); i < numCols; i++)
            if (array[mid][i] != value[i]) found = false;            // Control if the two rows are equal
        
        if(found) return mid;                                           // If row are equal at mid position then return the current index
        else {
            for (int i(0); i<numCols; i++) {
                if(array[mid][i] > value[i]) {high = mid; break;}    // If value row is smaller then array in mid check in the bot half

                if(array[mid][i] < value[i]) {low = mid; break;}     // Otherwise chel in the top half
            }
        }
    }
    return -1;                                                          // If the equality check always fail then return -1 as failure
}



// ---------------------------------------------------------------------------------------------------------------------
// Game simulation function: This function simulate the entire hand in all its phases, starting from the blind, flop, turn 
// then it will increment by 1 the winner hand, if draw occur the increment will be of 1/#Winner
bool playMatchSimulation() {

        std::vector<poker::card> deck_temp = poker::utility::getDeck();                                             // Obtaining a 52 cards deck (generated at initialization phase)

        // Constantly changing seed to avoid pattern in extraction            
        uint64_t newT = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (searchTime < newT) srand(searchTime / rand()%rand()), searchTime = newT;
        // Get a vector of 2 cards for each player
        std::vector<poker::hand> hands = poker::simulate::getPlayersHands(deck_temp, PLAYER_COUNT);

        std::vector<poker::card> board;                                                                             // Initialize board as a vector of cards
        std::vector<short> orderedTemp;                                                                             // Temporary ordered list
        std::vector<int> idxBlind, idxFlop, idxTurn;                                                                // Index container for each hands combinations
        int idx;                                                                                                    // Temporary index variable

        //------------------------------------------------------------------
        //--------------------------BLIND/FLOP------------------------------
        //------------------------------------------------------------------

        poker::simulate::getBoard(deck_temp, board, nFlop);                                                          // Draw the first 3 cards from the deck

        for (size_t i = 0; i < hands.size(); i++) {   

            poker::utility::getOrderedArray(hands[i], {}, orderedTemp);                                           // Order cards from hand and board toghether                             
            // Index binary search for 2 cards combination
            idx = handBinarySearch(combs2, orderedTemp, 0, sizeof(combs2)/sizeof(combs2[0]));                                 // Search the index from all the combinations
            oddsResultBlind[idx][1]++;                                                                              // Increase the hand count by one
            idxBlind.push_back(idx);                                                                                // store the index for future winner count

            poker::utility::getOrderedArray(hands[i], board, orderedTemp);                                        // Order cards from hand and board toghether 
            // Index binary search for 5 cards combination
            idx = handBinarySearch(combs5, orderedTemp, 0, sizeof(combs5)/sizeof(combs5[0]));                                 // Search the index from all the combinations
            oddsResultFlop[idx][1]++;                                                                               // Increase the hand count by one
            idxFlop.push_back(idx);                                                                                 // store the index for future winner count
        }

        //------------------------------------------------------------------
        //-----------------------------TURN---------------------------------
        //------------------------------------------------------------------

        poker::simulate::getBoard(deck_temp, board, nTurn);                                              // Draw the next card

        //------------------------------------------------------------------
        //----------------------------RIVER---------------------------------
        //------------------------------------------------------------------

        poker::simulate::getBoard(deck_temp, board, nRiver);                                             // Draw the last card
        
        std::vector<poker::rank> rankPlayer;                                                            // Hands rank list

        for (size_t i = 0; i < hands.size(); i++) {
            rankPlayer.push_back(poker::evaluate::rankHand(hands[i], board));                           // Store the rank for each player's hand
        }
        std::vector<int> results = poker::simulate::getWinner(rankPlayer);                              // End of game simulation result 

        // Result storing        
                                                                        
        for (size_t i(0); i < results.size(); i++) {                                                
        // Incrementing the winner combination in the games
        // If more than one player won on the same simulation the weight will be lower according to the winners count
            oddsResultBlind[idxBlind[results[i]]][0] += 1.f/results.size();
            oddsResultFlop[idxFlop[results[i]]][0] += 1.f/results.size();
        }
    count++;
    return true;
}



//Spawns n threads  
void spawnThreads(int n) {
    // Generating vector of threads according to the # of threads nThread
    std::vector<std::thread> threads(n);
    // spawn n threads:
    for (int i = 0; i < n; i++) {
        threads[i] = std::thread(playMatchSimulation);
    }

    for (auto& th : threads) {
        th.join();
    }
}




void writeResult() {

    std::cout << "\nLooking for older result files saved" << "\n";
    std::ifstream resultFile;

    for (std::string phase:GAME_PHASE) {                                   // Execute for each path inserted
        
        std::string path = "../saves/result" + phase +std::to_string(PLAYER_COUNT)+"p.txt";

        resultFile.clear();                                                 // Clearing the stream to avoid conflict
        resultFile.close();                                                 // Ensuring file stream is initially closed
        
        resultFile.open(path);                                              // Opening required file stream

        if(!resultFile) 
            std::cout << "No older file detected, creating new one at " << path << "\n";

        else {
            if (phase == "_blind_") {                           // Case 2 cards combinations list
                std::cout << "Loading: " << path << "\n";
                float r2;                                                       // Initialize value backup variable
                int idx = 0;                                                    // Initialize row index
                while (idx < COMBS_2CARD*2) {  
                    resultFile >> r2;                                           // Extracting card value
                    oddsResultBlind[idx/2][idx%2] += (r2);                      // Inserting at the corresponding position in the list
                    idx++;                                                      // Moving forward to the next variable
                } 
            std::remove(path.c_str());
            }
            if (phase == "_flop_") {                           // Case 5 cards combinations list
                std::cout << "Loading: " << path << "\n";
                float r5;                                                       // Initialize value backup variable
                int idx = 0;                                                    // Initialize row index
                while (idx < COMBS_5CARD*2) {  
                    resultFile >> r5;                                           // Extracting card value
                    oddsResultFlop[idx/2][idx%2] += (r5);                       // Inserting at the corresponding position in the list
                    idx++;                                                      // Moving forward to the next variable
                } 
            std::remove(path.c_str());
            }
        }
        resultFile.close();                                                 // Closing the reading stream
    }
    std::cout << "Result container successfully loaded!" << "\n\n";


    std::cout << "Writing final result: " << "\n";
    std::ofstream outResult;

    for (std::string phase:GAME_PHASE) {                                   // Execute for each path inserted
        
        std::string path = "../saves/result" + phase +std::to_string(PLAYER_COUNT)+"p.txt";

        outResult.open(path);

        if (phase == "_blind_") {
            std::cout << "- writing " << path << "...\n";
            for (size_t i(0); i < oddsResultBlind.size(); i++){
                outResult << std::to_string(oddsResultBlind[i][0]) << " " << std::to_string(oddsResultBlind[i][1]) << "\n";
            }
            outResult.close();
        }
        if (phase == "_flop_") {
            std::cout << "- writing " << path << "...\n";
            for (size_t i(0); i < oddsResultFlop.size(); i++){
                outResult << std::to_string(oddsResultFlop[i][0]) << " " << std::to_string(oddsResultFlop[i][1]) << "\n";
            }
            std::cout << "\n";
            outResult.close();
        }
    }
}

void repeatSim(int n) {
    for (int k = 0; k < n; k++) {
        try {                                                                       // Catch random bug and error (assuming the simulation has been already tested)

            // spawn and run #nThreads simulation
            spawnThreads(nThreads);
            
            double p = (((float)k/(n-1.f))*100);
            std::string progress = "Progress: " + std::to_string(p) + "%\t -> \tk=" + std::to_string(k) + "/"+ std::to_string(static_cast<int>(GAMES_NUMBER));
            std::cout << "\r" << progress;
        }
        catch(...) {
            k--;
        }
    }

    std::cout << "\nTotal games played: " << count << "\n";
            
    finalTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    printf("Total Elapsed Time: \t\t%2.3f\t[s]\n", (finalTime - initTime)/1000000000.f);
    printf("Total Loading Time: \t\t%2.3f\t[s]\n", (loadTime - initTime)/1000000000.f);
    printf("Total Simulation Time: \t\t%2.3f\t[s]\n", (finalTime - loadTime)/1000000000.f);
    printf("Average Single Simulation: \t%2.3f\t[\u03BCs]\n", (finalTime - loadTime)/count/1000.f);


    writeResult();
    exit(0);
}



void handler (int signum) {
    std::cout << "\n\nERROR!!!\n";       // Just report
    writeResult();

    std::cout << "\nTotal games played: " << count << "\n";
            
    uint64_t finalTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    printf("Total Elapsed Time: \t\t%2.3f\t[s]\n", (finalTime - initTime)/1000000000.f);
    printf("Total Loading Time: \t\t%2.3f\t[s]\n", (loadTime - initTime)/1000000000.f);
    printf("Total Simulation Time: \t\t%2.3f\t[s]\n", (finalTime - loadTime)/1000000000.f);
    printf("Average Single Simulation: \t%2.3f\t[\u03BCs]\n", (finalTime - loadTime)/count/1000.f);

    exit(1);
    std::cout << "\n Restarting:\n";

    //oddsResultBlind = 0;
    //std::fill(oddsResultBlind.begin(), oddsResultBlind.end(), 0);                    //  Resetting 2 cards result (blind)
    //std::fill(oddsResultFlop.begin(), oddsResultFlop.end(), 0);                      //  Resetting 5 cards result (flop)
    exit(1);

}


void intHandler (int signum) {
    std::cout << "\n\nINTERRUPTED!!!\n";       // Just report

    writeResult();

    std::cout << "\nTotal games played: " << count << "\n";
            
    uint64_t finalTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    printf("Total Elapsed Time: \t\t%2.3f\t[s]\n", (finalTime - initTime)/1000000000.f);
    printf("Total Loading Time: \t\t%2.3f\t[s]\n", (loadTime - initTime)/1000000000.f);
    printf("Total Simulation Time: \t\t%2.3f\t[s]\n", (finalTime - loadTime)/1000000000.f);
    printf("Average Single Simulation: \t%2.3f\t[\u03BCs]\n", (finalTime - loadTime)/count/1000.f);
    exit(1);
}

int main(int argc, char const *argv[]) {

    bool check = false;
    std::string input;
    while (!check){
        try{
            check = true;
            std::cout << "Choose total simulation to run: ";
            std::cin >> input;
            GAMES_NUMBER = std::stof(input);
        }   
        catch(std::invalid_argument) {
            std::cout << "Value not valid, please try again..." << "\n";
            check = false;
        }
    }
    check = false;
    while (!check){
        try{
            check = true;
            std::cout << "Choose total games to play per simulation (threads to be created): ";
            std::cin >> input;
            nThreads = std::stof(input);
        }   
        catch(std::invalid_argument) {
            std::cout << "Value not valid, please try again..." << "\n";
            check = false;
        }
    }
    check = false;
    while (!check){
        try{
            check = true;
            std::cout << "Choose the number of player (min:2, max:9): ";
            std::cin >> input;
            PLAYER_COUNT = std::stof(input);
            if (PLAYER_COUNT<2 || PLAYER_COUNT>9) 
                throw(PLAYER_COUNT);  
        }   
        catch(std::invalid_argument) {
            std::cout << "Value not valid, please try again..." << "\n";
            check = false;
        }
        catch(int PLAYER_COUNT) {
            std::cout << "You have chosen a total of " << PLAYER_COUNT << "players,\nplease select a values between 2 and 9...\n";
            check = false;
        }   
    }

    // setup error signal catch
    signal(SIGFPE, handler); 
    signal(SIGINT, intHandler);


        
    // Initialize initial time at the simulaiton start
    initTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();


    std::cout << "\nLoading combination container..." << "\n";
    std::vector<std::string> combsList = {"../saves/comb2c.txt", "../saves/comb5c.txt"};
    for (std::string path:combsList) {                                  // Execute for each path inserted

        combFile.clear();                                               // Clearing the stream to avoid conflict
        combFile.close();                                               // Ensuring file stream is initially closed
        // Loading paths informations
        combFile.open(path);                                            // Opening required file stream
        
        if(!combFile) {                                                 // Check if file has been found
            std::cout << "No files named " << path << " has been found, please check saves folder!" << "\n";
            exit(1);
        }
        if (path == "../saves/comb2c.txt") {                            // Case 2 cards combinations list
            std::cout << "Loading: " << path << "\n";
            short c2;                                                   // Initialize value backup variable
            int idx = 0;                                                // Initialize row index
            while (idx < COMBS_2CARD*2) {  
                combFile >> c2;                                         // Extracting card value
                combs2[idx/2][idx%2] = c2;                               // Inserting at the corresponding position in the list
                idx++;                                                  // Moving forward to the next variable
            } 
        }
        if (path == "../saves/comb5c.txt") {                            // Case 5 cards combinations list
            std::cout << "Loading: " << path << "\n";
            short c5;                                                   // Initialize value backup variable
            int idx = 0;                                                // Initialize row index
            while (idx < COMBS_5CARD*5) {  
                combFile >> c5;                                         // Extracting card value
                combs5[idx/5][idx%5] = c5;                                // Inserting at the corresponding position in the list
                idx++;                                                  // Moving forward to the next variable
            }
        }  
    }
    combFile.close();                                                   // closing combination file stream
    std::cout << "Combination container successfully loaded!" << "\n\n";


    // Saving total loading time wrt initial time
    loadTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // Binary search test 
    std::vector<short> test = {18,25,41,45,52};                                   // test numCPP vector
    std::cout << "Looking for [" << test[0] << ", " << test[1] << ", " << test[2] << ", " << test[3] << ", " << test[4] << "]\n";
    int foundIdx = handBinarySearch(combs5, test, 0, sizeof(combs5)/sizeof(combs5[0]));       // search  the values in the corresponding list
    std::cout << "Found at index: " << foundIdx << "\n";                            // Return the index found
    std::cout << "Result: " << combs5[foundIdx][0] << ", " << combs5[foundIdx][1] << ", " << combs5[foundIdx][2] << ", " << combs5[foundIdx][3] << ", " << combs5[foundIdx][4] << "]\n"; // Check if the vector is the same

    // Evaluating single test search time in microseconds
    searchTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "Search total time: " << (float)(searchTime-loadTime)/1000 << "[\u03BCs]\n" << "\n";

    // ------------------------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------------------------

    std::cout << "Start game..." << "\n";    

    srand(initTime);    // Initialize random seed

    std::cout << "Total threads created: " << nThreads << "\n";

    repeatSim(GAMES_NUMBER);

    return 0;
}
