/**
 * @file combinations.cpp
 * @author Mattia Scarpa (sct.mtt.25@gmail.com)
 * @brief implementation defining combination class functions 
 * @version 0.1
 * @date 2023-05-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>

#include "../include/combinations.hpp"


combinations::combinations(int player) :  combs2(COMBS_2CARD, std::vector<short>(2, 0)), 
                                combs5(COMBS_5CARD, std::vector<short>(5, 0)),
                                oddsResultBlind(COMBS_2CARD, std::vector<float>(2, 0)),
                                oddsResultFlop(COMBS_5CARD, std::vector<float>(2, 0)),
                                PLAYER_COUNT(player)
{   
    std::vector<std::string> paths = {"../saves/comb2c.txt", "../saves/comb5c.txt"};
    std::ifstream combFile;
    std::cout << "\nLoading combination container..." << "\n";
    long init =std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    for (std::string path : paths)
    {   // Execute for each path inserted

        combFile.clear();                                               // Clearing the stream to avoid conflict
        combFile.close();                                               // Ensuring file stream is initially closed
        // Loading paths informations
        combFile.open(path);                                            // Opening required file stream

        if(!combFile) {                                                 // Check if file has been found
            std::cout << "No files named " << path << " has been found, please check saves folder!" << "\n";
            exit(1);
        }
        if (path == "../saves/comb2c.txt") {                            // Case 2 cards combinations list
            std::cout << "Loading: " << path << "... ";
            short c2;                                                   // Initialize value backup variable
            int idx = 0;                                                // Initialize row index
            while (idx < COMBS_2CARD*2) {  
                combFile >> c2;                                         // Extracting card value
                combs2[idx/2][idx%2] = c2;                               // Inserting at the corresponding position in the list
                idx++;                                                  // Moving forward to the next variable
            } 
            std::cout << "SUCCESS!\n";
        }
        if (path == "../saves/comb5c.txt") {                            // Case 5 cards combinations list
            std::cout << "Loading: " << path << "... ";
            short c5;                                                   // Initialize value backup variable
            int idx = 0;                                                // Initialize row index
            while (idx < COMBS_5CARD*5) {  
                combFile >> c5;                                         // Extracting card value
                combs5[idx/5][idx%5] = c5;                                // Inserting at the corresponding position in the list
                idx++;                                                  // Moving forward to the next variable
            }
            std::cout << "SUCCESS!\n";
        }
    }
    combFile.close();                                                   // closing combination file stream
    long end =std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "Combination container successfully loaded in " << end-init << " ms" << "\n\n";
}

combinations::~combinations() {}

int combinations::binary_search(std::vector<std::vector<short>>& array, const std::vector<short>& value) 
{

    int numCols = value.size();        // Iterator index limit: the evaluation is made for all the elements in the row
    int low(0), high(array.size());

    while (low != high) {
        
        int mid = (low+high)/2;          

        bool found = true;
        for (size_t i(0); i < numCols; i++)
            if (array[mid][i] != value[i]) found = false;               // Control if the two rows are equal
        
        if(found) return mid;                                           // If row are equal at mid position then return the current index
        else {
            for (int i(0); i<numCols; i++) {
                if(array[mid][i] > value[i]) {high = mid; break;}       // If value row is smaller then array in mid check in the bot half

                if(array[mid][i] < value[i]) {low = mid; break;}        // Otherwise chel in the top half
            }
        }
    }
    return -1;                                                          // If the equality check always fail then return -1 as failure
}

int combinations::find_combinations(const std::vector<short>& target)
{   
    std::vector<std::vector<short>> combs;
    switch (target.size())
    {
    case 2:
        // execute binary search obtaining iterator
        return binary_search(combs2, target);
        break;
    case 5:
        // execute binary search obtaining iterator
        return binary_search(combs5, target);
        break;
    
    default:
        std::cerr << "No matching size provided, ensure your vector to be of 2 or 5 elements!\n";
        return -1;
    }
}

bool combinations::save_combinations()
{   
    if (PLAYER_COUNT < 2 || PLAYER_COUNT > 9) 
    {
        std::cerr << "Invalid player count!\n";
        return false;
    }
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
    return true;
}
