/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
----------------------------- IMPORTING LIBRARIES -----------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/

// Importing Standard Libraries

#include <iostream>
#include <vector>
#include <array>

// Importing Utilities Libraries

#include <math.h>
#include <assert.h>
#include <chrono>
#include <algorithm>

// Importing Custom Libraries

#include <poker.h>



/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
----------------------- Utility Functions Definitions -------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/

std::vector<poker::card> poker::utility::generateDeck() {
    std::vector<card> d;                                // Temporary deck
    for (size_t i(0); i < seedsCount; i++) {
        card c;
        c.seed = i;

        for (size_t j(0); j < valuesCount; j++) {
            c.value = j+2;
            d.push_back(c);
        }
    }
    deck = d;
    return deck;
}

short poker::utility::mapCard2Value(poker::card c)  {
    short val = c.seed*13 + c.value-1;      // = [0 to 3]*13 + [2 to 14]-1
    return val;
}

poker::card poker::utility::mapValue2Card(short s) {
    assert(1 <= s && s <= 52 && "Value must be included in the set [1, 52]");
    s--;                    // remapping the value from [1, 52] to [0, 51] to make the values easier to map
    card c;
    c.seed = s/13;
    c.value = s%13+2;
    return c;
}

std::string poker::utility::mapSymbol(const int s){
    switch (s) {        // Symbols values defined in MACRO symbol definition
    case 0:
        return H;       // 0 = Hearths -> "\xE2\x99\xA5"
        break;
    case 1:
        return D;       // 1 = Diamonds -> "\xE2\x99\xA6"
        break;
    case 2:
        return C;       // 2 = Clubs -> "\xE2\x99\xA3"
        break;
    case 3:
        return S;       // 3 = Spades -> "\xE2\x99\xA9"
        break;     
    default:            // If none of the expected seeds is found kill the program
        std::cerr << "Error seed conversion!" << "\n";
        exit(1);
    }
}

std::vector<poker::card> poker::utility::orderCards(std::vector<poker::card> uCards) {
    std::vector<card> oCards;           // ordered cards vector

    while (!uCards.empty()) {           // iterate until original cards vector has card

        int idx, min = 15;              // initialize min value to be larger than all the possible vard values
        for (size_t j(0); j < uCards.size(); j++) {     // iterate for all remaining elements
            if (uCards[j].value < min) {                // look for min values in the vector
                idx = j;                                // save the min index
                min = uCards[j].value;                  // save the min values
            }
        }
        oCards.push_back(poker::remove_at(uCards, idx));       // append to the ordered vector the value just found and remove it from the original vector
    }
    return oCards;
}

void poker::utility::printList(std::vector<poker::card> cardList) {
    for (size_t i = 0; i < cardList.size(); i++) {      // print every card [value][seed] in a row tab-spaced
        std::cout << cardList[i].value << utility::mapSymbol(cardList[i].seed) << "\t";
    }
    std::cout << "\n";
}

void poker::utility::printCard(poker::card card)  {
    std::cout << card.value << utility::mapSymbol(card.seed) << "\n";   // print the given card card [value][seed]
}

void poker::utility::getOrderedArray(const hand handSrc, const std::vector<card> boardSrc, std::vector<short> &v) {
    v.clear();                                                                 // Ensuring output vector is empty
    std::vector<card> cardList = boardSrc;                                      // Initialize the cards list to be evaluated with the board cards
    assert(handSrc.cards.size()==2 && "Hands card counts must be exactly 2");   // Hand cards must contain exactly 2 cards
    // Add the hand cards to the list
    cardList.push_back(handSrc.cards[0]);
    cardList.push_back(handSrc.cards[1]);
    for (size_t i(0); i < cardList.size(); i++) {
        v.push_back((cardList[i].value-1) + (cardList[i].seed*13));         // Saving ordered card list in NumCPP
    }    
    std::sort(v.begin(), v.end());                                     // Order card list by their deck position
}


/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
---------------------- Evaluation Functions Definitions -----------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/

bool poker::evaluate::isOnePair(const std::vector<card> cards, rank &hRank, int &pairVal) {  // Input: ordered vector cards, rank values container, pair backup variable
    bool found = false;                                                     // Setting as default pair not found
    pairVal = 0;                                                            // Pair backup values initialization
    int kickTemp = 0;                                                       // Kickers values
    int end(cards.size()-1);                                                // Index iterator starting from the list end
    int KICKER_COUNT = 3;                                                   // A pair hand has exactly 3 different values other than pair value
    // Looking for pairs
    for (size_t i(0); i < cards.size()-1; i++) {
        if (cards[i].value == cards[i+1].value) {
            found = true;                                                   // Set pair as found
            pairVal = cards[i].value;                                       // Store the pair value in a backup variable
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
                if (cards[i].value != pairVal) {                            // Kicker must be different from the pair found
                    kickTemp += cards[i].value*pow(15, 3-iK);               // Incrementing the valid kicker in descent weight order
                    iK++;                                                   // Next kicker weight less
                    KICKER_COUNT--;                                         // Decrement avilable kicker
                }
                i--;                                                        // Look backward
            }
            // Setting final hand rank
            hRank.value = 1;                                                // Setting final hand rank
            hRank.kickersValue = kickTemp;
            // Overall Power and kicker values
            hRank.OverallValue = hRank.value*pow(15,5) + hRank.kickersValue;
        }            
    return found;
}

bool poker::evaluate::isTwoPair(const std::vector<card> cards, rank &hRank) {                // Input: ordered vector cards, rank values container
    int kickTemp = 0;                                                       // Kickers values
    int idx(cards.size()-1);                                                // Pairs backup variables, index iterator
    //int MAX_SIZE = 1;                                                     // Since a two pair hand has exactly 1 different value other than pairs a sofisticated loop would be unnecessary
    int pairVal1 = 0, pairVal2 = 0;                                         // PairValues Backup
    // Looking for pairs
    for (size_t i(0); i < cards.size()-1; i++) {                            // For each element in the cards vector
        if (cards[i].value == cards[i+1].value) {
            pairVal2 = pairVal1;                                            // the previous pair (if already found otherwise is 0) is stored in
            pairVal1 = cards[i].value;                                      // Store the highest pair value in the first backup variable
        }            
    }
    // Increment from highest pairs values
    kickTemp += pairVal1*pow(15,4);
    kickTemp += pairVal2*pow(15,3);
    bool check = false;                                                     // initialize the kicker check condition                       
    while (!check) {
        if (cards[idx].value!=pairVal1 && cards[idx].value!=pairVal2) {     // Check for the highest non-pair values available
            kickTemp += cards[idx].value*pow(15,2);                         // The first 
            check = true;
        }
        idx--;                                                              // Looking backward
    }
    // Setting final hand rank and kickers
    hRank.value = 2;                                                        // Setting final hand rank
    hRank.kickersValue = kickTemp;
    // Overall Power and kicker values
    hRank.OverallValue = hRank.value*pow(15,5) + hRank.kickersValue;     
    return pairVal2;                                                        // pairVal2 != 0 iff a pair has been found more than twice
}

bool poker::evaluate::isTrips(const std::vector<card> cards, rank &hRank, int &trisVal) {    // Input: ordered vector cards, rank values container, trips backup variable
    bool found = false;                                                     // Setting as default pair not found
    int kickTemp = 0;                                                       // Kickers values
    int end(cards.size()-1);                                                // Index iterator
    int KICKER_COUNT = 2;                                                   // A tris hand has exactly 2 different values other than tris value
    trisVal = 0;                                                            // Tris backup values initialization
    // Looking for trips
    for (size_t i(0); i < cards.size()-2; i++) {
        // If current values is equal to next 2 values
        if (cards[i].value == cards[i+1].value && cards[i+1].value == cards[i+2].value) {     
            found = true;                                                   // Set trips as found
            trisVal = cards[i].value;                                       // Store the tris value in a backup variable
        }            
    }
    // Adding trips values
    if(found) {
        kickTemp += trisVal*pow(15,4);
        // Incrementing according to current kickers values                                                                        
        size_t i(end);                                                      // Moving backeard
        int iK = 0;
        while (KICKER_COUNT>0) {
            if (cards[i].value != trisVal) {                                // Kicker must be different from the tris found
                    kickTemp += cards[i].value*pow(15, 3-iK);               // Incrementing the valid kicker in descent weight order
                    iK++;                                                   // Next kicker weight less
                    KICKER_COUNT--;                                         // Decrement avilable kicker
            }
            i--;                                                            // Looking backward
        }
        // Setting final hand rank
        hRank.value = 3;                                                    // Setting final hand rank
        hRank.kickersValue = kickTemp;
        // Overall Power and kicker values
        hRank.OverallValue = hRank.value*pow(15,5) + hRank.kickersValue;      
    }            
    return found;
}

bool poker::evaluate::isStraight(const std::vector<card> cards, rank &hRank) {               // Input: ordered vector cards, rank values container
    std::vector<int> straightVal;                                           // Valid striaght cards container intialization
    // Aces are typically the highest valuable cards, however stairs also allow the sequence 
    // from A to 5, therefore is necessary to check if in the ordered cards list are present 
    // both ace and a duce are present respectivly at the end and at the beginning of the list

    if (cards[0].value == 2 && cards[cards.size()-1].value == 14)           // Check if there is a connection between and Ace and a two
        straightVal.push_back(cards[cards.size()-1].value);                 // If TRUE: add Ace as first card in the list          

    straightVal.push_back(cards[0].value);                                  // Add in the counter list the first card as beginner 
    // WARNING : è ok che iteri fino all'ultimo indice se poi deve fare il check con i+1? Eppure gira correttamente quindi... mistero da rivedere
    for (size_t i(0); i < cards.size(); i++) {                            // Mandatory: iterate for all cards 
        if (i+1 == cards.size() && straightVal.size() >=5) {                // If the check limit is reached and the count is >=5 a valid straight has been found
           // Setting final hand rank
           hRank.value = 4;                                                 // Setting final hand rank
           hRank.kickersValue = straightVal[straightVal.size()-1]*pow(15,4);// The highest stairs value is a sufficient kicker for straight hand
           // Overall Power and kicker values
           hRank.OverallValue = hRank.value*pow(15,5) + hRank.kickersValue;    
           return true;
        }
        if (cards[i].value == cards[i+1].value-1)                           // If the current values is equal to the next value less 1
            straightVal.push_back(cards[i+1].value);                       // The straight is still valid so add the next value to the list
        else if (cards[i].value == cards[i+1].value);                       // If the next value is constant do nothing and wait
        else if (straightVal.size() >= 5) {                                 // If the check fails and the count is >=5 the the only valid straight has been found
           // Setting final hand rank
           hRank.value = 4;                                                 // Setting final hand rank
           hRank.kickersValue = straightVal[straightVal.size()-1]*pow(15,4);// The highest stairs value is a sufficient kicker for straight hand
           // Overall Power and kicker values
           hRank.OverallValue = hRank.value*pow(15,5) + hRank.kickersValue;    
           return true;
        }                   
        else {  // WARNING: Extra early stop can be added
            straightVal.clear();   
            straightVal.push_back(cards[i+1].value);                        // Otherwise reset and restart the count from the next value
        }
    }
    return false;
}

bool poker::evaluate::isFlush(const std::vector<card> cards, rank &hRank) {                  // Input: ordered vector cards, rank values container
    std::vector<std::vector<card>> cardsBin{{},{},{},{}};                   // Generating bin seeds container
    int KICKER_COUNT = 0;                                                   // Kickers values count
    int MAX_COUNT = 5;                                                      // Kickers values count cannot exeed 5


    for (size_t i(0); i < cards.size(); i++) {
        cardsBin[cards[i].seed].push_back(cards[i]);                        // Adding card in its corresponding seed bin
    }

    for (size_t i(0); i < cardsBin.size(); i++) {                           // Checking all bins
        if (cardsBin[i].size() >= 5) {
            hRank.value = 5;                                                // Setting final hand rank
            int idx = cardsBin[i].size()-1;                                 // Starting from the end of the list
            while (KICKER_COUNT < MAX_COUNT) {                              // Adding Kickers values
                // Storing kickers values
                hRank.kickersValue += cardsBin[i][idx].value*pow(15,4-KICKER_COUNT);  
                KICKER_COUNT++;                                             // Increasing the count of kicker evaluated
                idx--;                                                      // Looking backward
            }                              
            // Overall Power and kicker values
            hRank.OverallValue = hRank.value*pow(15,5) + hRank.kickersValue; 
            return true;
        }
    }
    return false;
}

bool poker::evaluate::isFullHouse(const std::vector<card> cards, rank &hRank) {              // Input: ordered vector cards, rank values container
    int trisKicker = 0, pairKicker = 0;
    if(isTrips(cards, hRank, trisKicker)) {                                 // Check the tris existence 
        std::vector<card> temp = remove(cards, trisKicker);                 // Remove the tris values
        if (isOnePair(temp, hRank, pairKicker)) {                           // If also a pair exist implies full house
            hRank.value = 6;                                                // Setting final hand rank
            hRank.kickersValue = 0;                                         // Resetting kickers values from previous callback
            hRank.kickersValue += trisKicker*pow(15,4);                     // Tris kicker has highest weight
            hRank.kickersValue += pairKicker*pow(15,3);                     // Pair kicker weight
            // Overall Power and kicker values                         
            hRank.OverallValue = hRank.value*pow(15,5) + hRank.kickersValue; 
            return true;
        }
    }
    return false;
}

bool poker::evaluate::isQuad(const std::vector<card> cards, rank &hRank) {                   // Input: ordered vector cards, rank values container
    for (size_t i(0); i < cards.size()-3; i++) {
        // If the next three values are equal with the current
        if(cards[i].value == cards[i+1].value && cards[i+1].value == cards[i+2].value && cards[i+2].value == cards[i+3].value) {
            hRank.value = 7;                                                // Setting final hand rank
            hRank.kickersValue = cards[i].value*pow(15,4);                  // Poker value is the most valuable kicker
            if (i < cards.size()-4)                                         // Look if there is other cards (in the ordered list) after the poker values
                hRank.kickersValue += cards[cards.size()-1].value*pow(15,3);
            else                                                            // Otherwise the kicker values is given by the last non-poker card
                hRank.kickersValue += cards[i-1].value*pow(15,3);
            // Overall Power and kicker values                         
            hRank.OverallValue = hRank.value*pow(15,5) + hRank.kickersValue; 
            return true;
        }
    }
    return false;
}

bool poker::evaluate::isStraightFlush(const std::vector<card> cards, rank &hRank) {          // Input: ordered vector cards, rank values container
    std::vector<std::vector<card>> cardsBin {{},{},{},{}};                  // Generating bin seeds container
    // Iterating for each card
    for (size_t i(0); i < cards.size(); i++) {
        cardsBin[cards[i].seed].push_back(cards[i]);                        // Fill each bin with respective seeds
    }
    for (size_t i = 0; i < cardsBin.size(); i++) {
        if (cardsBin[i].size() >= 5) {                                      // If flush it must has more than 5 cards
            if(isStraight(cardsBin[i], hRank)) {                            // If also a stairs then it's a stright flush
                hRank.value = 8;                                            // Adjusting hands rank
                // Straight and striaght flush share the same kickers so no additional adjustment are required for it
                // Overall Power and kicker values                         
                hRank.OverallValue = hRank.value*pow(15,5) + hRank.kickersValue; 
                return true;
            }
        }
    }
    return false;
}

poker::rank poker::evaluate::rankHand(const hand handSrc, const std::vector<card> boardSrc, bool printBoard) {
                
    rank hRank;                                                                 // Initialize hand rank variable
    int backup = 0;                                                             // Backup variable (required for one pair and tris)
    std::vector<card> cardList = boardSrc;                                      // Initialize the cards list to be evaluated with the board cards
    assert(handSrc.cards.size()==2 && "Hands card counts must be exactly 2");   // Hand cards must contain exactly 2 cards

    // Add the hand cards to the list
    cardList.push_back(handSrc.cards[0]);
    cardList.push_back(handSrc.cards[1]);

    cardList = utility::orderCards(cardList);                                   // Order cards list (mandatory for the correct evaluation)

    if(printBoard && cardList.size()==7)
        utility::printList(cardList);
            
    if (isStraightFlush(cardList, hRank)) return hRank;                         // Straight flush check
    if (isQuad(cardList, hRank)) return hRank;                                  // Quads check           
    if (isFullHouse(cardList, hRank)) return hRank;                             // Full house check            
    if (isFlush(cardList, hRank)) return hRank;                                 // Flush check
    if (isStraight(cardList, hRank)) return hRank;                              // Straight check
    if (isTrips(cardList, hRank, backup)) return hRank;                         // Trips check
    if (isTwoPair(cardList, hRank)) return hRank;                               // Two Pair check
    if (isOnePair(cardList, hRank, backup)) return hRank;                       // One Pair check
    // Setting high cards default value if nothing else has been found
    hRank.value = 0;                                                            // Setting final hand rank
    for (size_t i(1); i <= 5; i++) {
        hRank.kickersValue += cardList[cardList.size()-i].value*pow(15,5-i);
    }
    // Overall Power and kicker values              
    hRank.OverallValue = hRank.value*pow(15,5) + hRank.kickersValue; 
    return hRank;
}

bool poker::evaluate::rankHand(size_t n, const short combList[], const std::vector<card> boardSrc, hand h, rank &hRank, bool printBoard) {

    int backup = 0;                                                             // Backup variable (required for one pair and tris)
    std::vector<card> cardList = boardSrc;                                      // Initialize the cards list to be evaluated with the board cards
            
    for (size_t i = 0; i < n; i++) {
        card c = utility::mapValue2Card(combList[i]);
        if (include(c, h.cards))
            return false;
        if (include(c, cardList))
            return false;
        cardList.push_back(c);
    }            

    cardList = utility::orderCards(cardList);                                   // Order cards list (mandatory for the correct evaluation)

    if(printBoard && cardList.size()==7)
        utility::printList(cardList);
            
    if (isStraightFlush(cardList, hRank)) return true;                          // Straight flush check
    if (isQuad(cardList, hRank)) return true;                                   // Quads check
    if (isFullHouse(cardList, hRank)) return true;                              // Full house check
    if (isFlush(cardList, hRank)) return true;                                  // Flush check
    if (isStraight(cardList, hRank)) return true;                               // Straight check
    if (isTrips(cardList, hRank, backup)) return true;                          // Trips check
    if (isTwoPair(cardList, hRank)) return true;                                // Two Pair check
    if (isOnePair(cardList, hRank, backup)) return true;                        // One Pair check
            
    // Setting high cards default value if nothing else has been found
    hRank.value = 0;                                                            // Setting final hand rank
    for (size_t i(1); i <= 5; i++) 
        hRank.kickersValue += cardList[cardList.size()-i].value*pow(15,5-i);
    // Overall Power and kicker values              
    hRank.OverallValue = hRank.value*pow(15,5) + hRank.kickersValue; 
    return true;
}


/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
---------------------- Simulation Functions Definitions -----------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/

std::vector<poker::hand> poker::simulate::getPlayersHands(std::vector<poker::card> &deckSrc, const int count) {    // Input: deck source, # of player in the game
    // Generating seed for random extraction from current time expressed in nanosecond
 
    uint64_t seed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    srand(seed);
    std::vector<poker::hand> hands;                                          // Genereting hands list
    for (size_t i(0); i < count; i++) {                                     // For each hands count
        poker::hand hand;                                                   // Single hand container
        hand.cards.clear();
        hand.cards.push_back(poker::remove_at(deckSrc, rand()%deckSrc.size()));    // First card extraction
        hand.cards.push_back(poker::remove_at(deckSrc, rand()%deckSrc.size()));    // Second card extraction
        assert(hand.cards.size()==2 && "Hands card counts must be exactly 2");
        hands.push_back(hand);                                              // Add current hand in the hands list
    }   
    return hands;
}

void poker::simulate::getBoard(std::vector<poker::card> &deckSrc, std::vector<poker::card> &board, int cBoard) {
    // Generating seed for random extraction from current time expressed in nanosecond
    uint64_t seed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    srand(seed);
    for (size_t i = 0; i < cBoard; i++) {                                   // For each card in the board
        board.push_back(poker::remove_at(deckSrc, rand()%deckSrc.size()));  // Draw randomly from the card deck
    }
}

std::vector<int> poker::simulate::getWinner(std::vector<rank> handRanks) {                   // Input: Rank player values list

    std::vector<int> winners;                                               // Initialize winners position list
    int maxValue = 0;                                                       // Initialize hand values backup variable
            
    for (size_t i = 0; i < handRanks.size(); i++) {
        if(handRanks[i].OverallValue > maxValue) {                          // If an highest values is found
            winners.clear();                                                // Clear the winners vector
            winners.push_back(i);                                           // Add the winner position
            maxValue = handRanks[i].OverallValue;                           // Update the max value for the next control
        } else if(handRanks[i].OverallValue == maxValue) {                  // If the current hand value is the same of the current max value
            winners.push_back(i);                                           // Adding the current position to the winners list
        }
    }
    return winners;
}

float poker::simulate::isWinner(rank p1, rank p2) {                   // Input: rank player 1, rank player 2            

    if (p1.OverallValue > p2.OverallValue)
        return 1;
    if (p1.OverallValue == p2.OverallValue)
        return 1.f/2;
    return 0;
}