
#include <iostream>
#include <memory>
#include <chrono>
#include <algorithm>
#include <vector>
#include <math.h>

#include "../include/utilities.hpp"
#include "../include/card.hpp"

#include "../include/hand.hpp"

namespace poker
{
    constexpr int FLOP{3};
    constexpr int TURN{4};
    constexpr int RIVER{5};

    /***************************************************************************************
     ***************************************************************************************
     ************************************ CONSTRUCTORS *************************************
     ***************************************************************************************
     ***************************************************************************************/
    
    hand::hand(card c1, card c2)
    {   
        if (c1==c2)
            throw std::invalid_argument("Cards must be different!");
        
        Hand.push_back(c1);
        Hand.push_back(c2);
    }

    hand::hand(short v1, short v2)
    {
        Hand.push_back(card(v1));
        Hand.push_back(card(v2));
    }

    hand::~hand() {}

    /***************************************************************************************
     ***************************************************************************************
     ************************************** FUNCTIONS **************************************
     ***************************************************************************************
     ***************************************************************************************/

    std::vector<card> hand::order_cards()
    {
        // initialize ordered cards vector buffer
        std::vector<card> ordered_Cards;
        // merge hand and board into an unordered vector
        std::vector<card> Cards(Hand.begin(), Hand.end());
        Cards.insert(Cards.end(), Board->begin(), Board->end());

        std::sort(Cards.begin(), Cards.end());
        return Cards;
    }

    bool hand::set_board(std::vector<card>* board) 
    {   
        // board reference cannot be overwritten before dedicated function
        if (Board_valid)
        {
            std::cerr << "Flop insertion failed, you may want to use 'clear_board' function before\n";
            return false;
        }
        Board = board;
        Board_valid = true;
        return true;
    }

    bool hand::isOnePair(int &pairVal)
    {   // Input: ordered vector cards, rank values container, pair backup variable
        bool found = false;                                                     // Setting as default pair not found
        pairVal = 0;                                                            // Pair backup values initialization
        int kickTemp = 0;                                                       // Kickers values
        int end(Ordered_Card_List.size()-1);                                    // Index iterator starting from the list end
        int KICKER_COUNT = 3;                                                   // A pair hand has exactly 3 different values other than pair value
        // Looking for pairs
        for (size_t i(0); i < Ordered_Card_List.size()-1; i++) {
            if (Ordered_Card_List[i].get_card().value == Ordered_Card_List[i+1].get_card().value) {
                found = true;                                                   // Set pair as found
                pairVal = Ordered_Card_List[i].get_card().value;                // Store the pair value in a backup variable
            }            
        }
            if (found) {                                                        // If a pair has been found
                // Adding highest pair value
                kickTemp = pairVal*pow(15,4);                                  // Pair values is the first kicker
                                                                                // so its base15 encoding has the highest weigth 15^4
                // Incrementing according to current kickers values                                                                        
                size_t i(end);
                int iK = 0;                                                     // Index to decrement the kicker weight
                while (KICKER_COUNT>0) {
                    // Kicker must be different from the pair found
                    if (Ordered_Card_List[i].get_card().value != pairVal) {
                        // Incrementing the valid kicker in descent weight order
                        kickTemp += Ordered_Card_List[i].get_card().value*pow(15, 3-iK);    
                        iK++;                                                   // Next kicker weight less
                        KICKER_COUNT--;                                         // Decrement avilable kicker
                    }
                    i--;                                                        // Look backward
                }
                // Setting final hand rank
                Rank.value = 1;                                                // Setting final hand rank
                Rank.kickersValue = kickTemp;
                // Overall Power and kicker values
                Rank.OverallValue = Rank.value*pow(15,5) + Rank.kickersValue;
            }            
        return found;
    }

    bool hand::isOnePair(std::vector<card> Card_List, int &pairVal)
    {   // Input: ordered vector cards, rank values container, pair backup variable
        bool found = false;                                                     // Setting as default pair not found
        pairVal = 0;                                                            // Pair backup values initialization
        int kickTemp = 0;                                                       // Kickers values
        int end(Card_List.size()-1);                                    // Index iterator starting from the list end
        int KICKER_COUNT = 3;                                                   // A pair hand has exactly 3 different values other than pair value
        // Looking for pairs
        for (size_t i(0); i < Card_List.size()-1; i++) {
            if (Card_List[i].get_card().value == Card_List[i+1].get_card().value) {
                found = true;                                           // Set pair as found
                pairVal = Card_List[i].get_card().value;                // Store the pair value in a backup variable
            }            
        }
        return found;
    }

    bool hand::isTwoPair()
    {   // Input: ordered vector cards, rank values container
        int kickTemp = 0;                                                       // Kickers values
        int idx(Ordered_Card_List.size()-1);                                                // Pairs backup variables, index iterator
        //int MAX_SIZE = 1;                                                     // Since a two pair hand has exactly 1 different value other than pairs a sofisticated loop would be unnecessary
        int pairVal1 = 0, pairVal2 = 0;                                         // PairValues Backup
        // Looking for pairs
        for (size_t i(0); i < Ordered_Card_List.size()-1; i++) {                            // For each element in the cards vector
            if (Ordered_Card_List[i].get_card().value == Ordered_Card_List[i+1].get_card().value) {
                pairVal2 = pairVal1;                                            // the previous pair (if already found otherwise is 0) is stored in
                pairVal1 = Ordered_Card_List[i].get_card().value;                                      // Store the highest pair value in the first backup variable
            }            
        }
        // Increment from highest pairs values
        kickTemp += pairVal1*pow(15,4);
        kickTemp += pairVal2*pow(15,3);
        bool check = false;                                                     // initialize the kicker check condition                       
        while (!check) {
            // Check for the highest non-pair values available
            if (Ordered_Card_List[idx].get_card().value!=pairVal1 && Ordered_Card_List[idx].get_card().value!=pairVal2) { 
                kickTemp += Ordered_Card_List[idx].get_card().value*pow(15,2);                         // The first 
                check = true;
            }
            idx--;                                                              // Looking backward
        }
        // Setting final hand rank and kickers
        Rank.value = 2;                                                        // Setting final hand rank
        Rank.kickersValue = kickTemp;
        // Overall Power and kicker values
        Rank.OverallValue = Rank.value*pow(15,5) + Rank.kickersValue;     
        return pairVal2;
    }

    bool hand::isTrips(int &trisVal)
    {// Input: ordered vector cards, rank values container, trips backup variable
        bool found = false;                                                     // Setting as default pair not found
        int kickTemp = 0;                                                       // Kickers values
        int end(Ordered_Card_List.size()-1);                                                // Index iterator
        int KICKER_COUNT = 2;                                                   // A tris hand has exactly 2 different values other than tris value
        trisVal = 0;                                                            // Tris backup values initialization
        // Looking for trips
        for (size_t i(0); i < Ordered_Card_List.size()-2; i++) {
            // If current values is equal to next 2 values
            if (Ordered_Card_List[i].get_card().value == Ordered_Card_List[i+1].get_card().value && Ordered_Card_List[i+1].get_card().value == Ordered_Card_List[i+2].get_card().value) {     
                found = true;                                                   // Set trips as found
                trisVal = Ordered_Card_List[i].get_card().value;                                       // Store the tris value in a backup variable
            }            
        }
        // Adding trips values
        if(found) {
            kickTemp += trisVal*pow(15,4);
            // Incrementing according to current kickers values                                                                        
            size_t i(end);                                                      // Moving backeard
            int iK = 0;
            while (KICKER_COUNT>0) {
                if (Ordered_Card_List[i].get_card().value != trisVal) {                                // Kicker must be different from the tris found
                        kickTemp += Ordered_Card_List[i].get_card().value*pow(15, 3-iK);               // Incrementing the valid kicker in descent weight order
                        iK++;                                                   // Next kicker weight less
                        KICKER_COUNT--;                                         // Decrement avilable kicker
                }
                i--;                                                            // Looking backward
            }
            // Setting final hand rank
            Rank.value = 3;                                                    // Setting final hand rank
            Rank.kickersValue = kickTemp;
            // Overall Power and kicker values
            Rank.OverallValue = Rank.value*pow(15,5) + Rank.kickersValue;      
        }            
        return found;
    }

    bool hand::isStraight() 
    {   // Input: ordered vector cards, rank values container
        std::vector<int> straightVal;                                           // Valid striaght cards container intialization
        // Aces are typically the highest valuable cards, however stairs also allow the sequence 
        // from A to 5, therefore is necessary to check if in the ordered cards list are present 
        // both ace and a duce are present respectivly at the end and at the beginning of the list

        if (Ordered_Card_List[0].get_card().value == 2 && Ordered_Card_List[Ordered_Card_List.size()-1].get_card().value == 14)           // Check if there is a connection between and Ace and a two
            straightVal.push_back(Ordered_Card_List[Ordered_Card_List.size()-1].get_card().value);                 // If TRUE: add Ace as first card in the list          

        straightVal.push_back(Ordered_Card_List[0].get_card().value);                                  // Add in the counter list the first card as beginner 
        // WARNING : è ok che iteri fino all'ultimo indice se poi deve fare il check con i+1? Eppure gira correttamente quindi... mistero da rivedere
        for (size_t i(0); i < Ordered_Card_List.size(); i++) {                            // Mandatory: iterate for all cards 
            if (i+1 == Ordered_Card_List.size() && straightVal.size() >=5) {                // If the check limit is reached and the count is >=5 a valid straight has been found
               // Setting final hand rank
               Rank.value = 4;                                                 // Setting final hand rank
               Rank.kickersValue = straightVal[straightVal.size()-1]*pow(15,4);// The highest stairs value is a sufficient kicker for straight hand
               // Overall Power and kicker values
               Rank.OverallValue = Rank.value*pow(15,5) + Rank.kickersValue;    
               return true;
            }
            if (Ordered_Card_List[i].get_card().value == Ordered_Card_List[i+1].get_card().value-1)                           // If the current values is equal to the next value less 1
                straightVal.push_back(Ordered_Card_List[i+1].get_card().value);                       // The straight is still valid so add the next value to the list
            else if (Ordered_Card_List[i].get_card().value == Ordered_Card_List[i+1].get_card().value);                       // If the next value is constant do nothing and wait
            else if (straightVal.size() >= 5) {                                 // If the check fails and the count is >=5 the the only valid straight has been found
               // Setting final hand rank
               Rank.value = 4;                                                 // Setting final hand rank
               Rank.kickersValue = straightVal[straightVal.size()-1]*pow(15,4);// The highest stairs value is a sufficient kicker for straight hand
               // Overall Power and kicker values
               Rank.OverallValue = Rank.value*pow(15,5) + Rank.kickersValue;    
               return true;
            }                   
            else {  // WARNING: Extra early stop can be added
                straightVal.clear();   
                straightVal.push_back(Ordered_Card_List[i+1].get_card().value);                        // Otherwise reset and restart the count from the next value
            }
        }
        return false;
    }

    bool hand::isStraight(std::vector<card> Card_List) 
    {   // Input: ordered vector cards, rank values container
        std::vector<int> straightVal;                                           // Valid striaght cards container intialization
        // Aces are typically the highest valuable cards, however stairs also allow the sequence 
        // from A to 5, therefore is necessary to check if in the ordered cards list are present 
        // both ace and a duce are present respectivly at the end and at the beginning of the list

        if (Card_List[0].get_card().value == 2 && Card_List[Card_List.size()-1].get_card().value == 14)           // Check if there is a connection between and Ace and a two
            straightVal.push_back(Card_List[Card_List.size()-1].get_card().value);                 // If TRUE: add Ace as first card in the list          

        straightVal.push_back(Card_List[0].get_card().value);                                  // Add in the counter list the first card as beginner 
        // WARNING : è ok che iteri fino all'ultimo indice se poi deve fare il check con i+1? Eppure gira correttamente quindi... mistero da rivedere
        for (size_t i(0); i < Card_List.size(); i++) {                            // Mandatory: iterate for all cards 
            if (i+1 == Card_List.size() && straightVal.size() >=5) {                // If the check limit is reached and the count is >=5 a valid straight has been found
               // Setting final hand rank
               Rank.value = 4;                                                 // Setting final hand rank
               Rank.kickersValue = straightVal[straightVal.size()-1]*pow(15,4);// The highest stairs value is a sufficient kicker for straight hand
               // Overall Power and kicker values
               Rank.OverallValue = Rank.value*pow(15,5) + Rank.kickersValue;    
               return true;
            }
            if (Card_List[i].get_card().value == Card_List[i+1].get_card().value-1)                           // If the current values is equal to the next value less 1
                straightVal.push_back(Card_List[i+1].get_card().value);                       // The straight is still valid so add the next value to the list
            else if (Card_List[i].get_card().value == Card_List[i+1].get_card().value);                       // If the next value is constant do nothing and wait
            else if (straightVal.size() >= 5) {                                 // If the check fails and the count is >=5 the the only valid straight has been found
               // Setting final hand rank
               Rank.value = 4;                                                 // Setting final hand rank
               Rank.kickersValue = straightVal[straightVal.size()-1]*pow(15,4);// The highest stairs value is a sufficient kicker for straight hand
               // Overall Power and kicker values
               Rank.OverallValue = Rank.value*pow(15,5) + Rank.kickersValue;    
               return true;
            }
            else {  // WARNING: Extra early stop can be added
                straightVal.clear();   
                straightVal.push_back(Card_List[i+1].get_card().value);                        // Otherwise reset and restart the count from the next value
            }
        }
        return false;
    }

    bool hand::isFlush() 
    {   // Input: ordered vector cards, rank values container
        std::vector<std::vector<card>> cardsBin{{},{},{},{}};                   // Generating bin seeds container
        int KICKER_COUNT = 0;                                                   // Kickers values count
        int MAX_COUNT = 5;                                                      // Kickers values count cannot exeed 5


        for (size_t i(0); i < Ordered_Card_List.size(); i++) {
            // Adding card i in its corresponding seed bin
            cardsBin[Ordered_Card_List[i].get_card().seed].push_back(Ordered_Card_List[i]);
        }

        for (size_t i(0); i < cardsBin.size(); i++) {                           // Checking all bins
            if (cardsBin[i].size() >= 5) {
                Rank.value = 5;                                                // Setting final hand rank
                int idx = cardsBin[i].size()-1;                                 // Starting from the end of the list
                while (KICKER_COUNT < MAX_COUNT) {                              // Adding Kickers values
                    // Storing kickers values
                    Rank.kickersValue += cardsBin[i][idx].get_card().value*pow(15,4-KICKER_COUNT);  
                    KICKER_COUNT++;                                             // Increasing the count of kicker evaluated
                    idx--;                                                      // Looking backward
                }                              
                // Overall Power and kicker values
                Rank.OverallValue = Rank.value*pow(15,5) + Rank.kickersValue; 
                return true;
            }
        }
        return false;
    }

    bool hand::isFullHouse()
    {   // Input: ordered vector cards, rank values container
        int trisKicker = 0, pairKicker = 0;
        if(isTrips(trisKicker)) {                               // Check the tris existence 
            std::vector<card> temp = remove(trisKicker);        // Remove the tris values
            if (isOnePair(temp, pairKicker)) {                  // If also a pair exist implies full house
                Rank.value = 6;                                 // Setting final hand rank
                Rank.kickersValue = 0;                          // Resetting kickers values from previous callback
                Rank.kickersValue += trisKicker*pow(15,4);      // Tris kicker has highest weight
                Rank.kickersValue += pairKicker*pow(15,3);      // Pair kicker weight
                // Overall Power and kicker values                         
                Rank.OverallValue = Rank.value*pow(15,5) + Rank.kickersValue; 
                return true;
            }
        }
        return false;
    }

    bool hand::isQuad()
    {   // Input: ordered vector cards, rank values container
        for (size_t i(0); i < Ordered_Card_List.size()-3; i++) {
            // If the next three values are equal with the current
            if(Ordered_Card_List[i].get_card().value == Ordered_Card_List[i+1].get_card().value && Ordered_Card_List[i+1].get_card().value == Ordered_Card_List[i+2].get_card().value && Ordered_Card_List[i+2].get_card().value == Ordered_Card_List[i+3].get_card().value) {
                Rank.value = 7;                                                // Setting final hand rank
                Rank.kickersValue = Ordered_Card_List[i].get_card().value*pow(15,4);                  // Poker value is the most valuable kicker
                if (i < Ordered_Card_List.size()-4)                                         // Look if there is other cards (in the ordered list) after the poker values
                    Rank.kickersValue += Ordered_Card_List[Ordered_Card_List.size()-1].get_card().value*pow(15,3);
                else                                                            // Otherwise the kicker values is given by the last non-poker card
                    Rank.kickersValue += Ordered_Card_List[i-1].get_card().value*pow(15,3);
                // Overall Power and kicker values                         
                Rank.OverallValue = Rank.value*pow(15,5) + Rank.kickersValue; 
                return true;
            }
        }
        return false;
    }

    bool hand::isStraightFlush()
    {   // Input: ordered vector cards, rank values container
        std::vector<std::vector<card>> cardsBin {{},{},{},{}};                  // Generating bin seeds container
        // Iterating for each card
        for (size_t i(0); i < Ordered_Card_List.size(); i++) {
            cardsBin[Ordered_Card_List[i].get_card().seed].push_back(Ordered_Card_List[i]);                        // Fill each bin with respective seeds
        }
        for (size_t i = 0; i < cardsBin.size(); i++) {
            if (cardsBin[i].size() >= 5) {                                      // If flush it must has more than 5 cards
                if(isStraight(cardsBin[i])) {                            // If also a stairs then it's a stright flush
                    Rank.value = 8;                                            // Adjusting hands rank
                    // Straight and striaght flush share the same kickers so no additional adjustment are required for it
                    // Overall Power and kicker values                         
                    Rank.OverallValue = Rank.value*pow(15,5) + Rank.kickersValue; 
                    return true;
                }
            }
        }
        return false;
    }


    bool hand::rank_hand()
    {
        if (!Board_valid)
        {
            std::cerr << "No valid board is provided, have you used the function 'set_board'?\n";
            return false;
        }
        if (Board->size() < FLOP || Board->size() > RIVER)
        {
            std::cerr << "Invalid board size, it must be 3,4 or 5\n";
            return false;
        }
        
        int backup(0);                  // Backup variable (required for one pair and tris)
        Ordered_Card_List = order_cards();

        //get_sorted_position(); 

        if (isStraightFlush()) return true;                         // Straight flush check
        if (isQuad()) return true;                                  // Quads check           
        if (isFullHouse()) return true;                             // Full house check            
        if (isFlush()) return true;                                 // Flush check
        if (isStraight()) return true;                              // Straight check
        if (isTrips(backup)) return true;                           // Trips check
        if (isTwoPair()) return true;                               // Two Pair check
        if (isOnePair(backup)) return true;                         // One Pair check

        // If nothing is found set the high card rank
        Rank.value = 0;
        for (size_t i(1); i <= 5; i++) {
            Rank.kickersValue += Ordered_Card_List[Ordered_Card_List.size()-i].get_card().value*pow(15,5-i);
        }
        Rank.OverallValue = Rank.value*pow(15,5) + Rank.kickersValue; 
        return true;
    }
} // namespace poker
