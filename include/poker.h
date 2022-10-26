#ifndef POKER_H
#define POKER_H

/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
--------------------------- DEFINE SYMBOLS VALUES -----------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/

#define valuesCount 13         // # cards values
#define seedsCount  4          // # seeds count

// Seeds Symbols mapping
#define H "\xE2\x99\xA5";      // Hearts Symbol
#define D "\xE2\x99\xA6";      // Diamonds Symbol
#define C "\xE2\x99\xA3";      // Clubs Symbol
#define S "\xE2\x99\xA0";      // Spades Symbol



/*
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
----------------------------- FUNCTIONS DEFINITION ----------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
*/

namespace poker {   // main functions group



    // Data Structure Declaration

    struct card {
        // value between 2-14 (i.e. 2-3-4-5-6-7-8-9-10-J-Q-K-A)
        short value = 0;  
        // seed (0 - heart (H), 1 - diamonds (D), 2 - club (C), 3 - spades (S)) 
        short seed = 0;    
    };

    struct hand {
        std::vector<card> cards;
    };

    struct rank {
        // Hand "Power", if -1 rank not defined yet
        // 0 - High Card    /    1 - Pair       /   2 - Two Pair
        // 3 - Trips        /    4 - Straight   /   5 - Flush
        // 6 - Full House   /    7 - Quads      /   8 - Straight Flush
        short value = -1;  
        // Kickers values (initialized at 0): computed differently for each hand according to a base 15 encoding:
        // = 15^4*[0] + 15^3*[1] + 15^2*[2] + 15*[3] + [4]  
        // where [i] is the i-th kickersValue included in [0 to 12]   
        int kickersValue = 0;   
        // = 15^5*[value] + [kickersValue]     
        int OverallValue = 0;   
    };

    inline std::vector<card> deck;         // Hidden deck, possible to access only with dedicated function

    inline bool operator== (const card &c1, const card &c2) {
        return c1.seed == c2.seed && c1.value == c2.value;
    }

    // ------------------------------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------------------------------


    namespace {


        // Remove element at specified index from a vector
        template <typename T> T remove_at(std::vector<T>&v, typename std::vector<T>::size_type n) {
            T ans = std::move_if_noexcept(v[n]);
            v[n] = std::move_if_noexcept(v.back());
            v.pop_back();
            return ans;
        }

    }


    namespace utility {     // utilities functions

        // Deck generation: 52 cards - 13*4 -> [2 to 14] values for [0 to 3] seeds
        std::vector<card> generateDeck(); 

        // Card Values mapping: return the values from 1 to 52 according to the card deck position
        // Aces are considered the highest values for each seed
        short mapCard2Value(card c);

        /*
            All the values living in the interval [1, 52] can be mapped to a card data structure under some strict assumption

            1 - the card values go from 2 to 14 -> 2, 3, 4, 5, 6, 7, 8, 9, T(10), J(11), Q(12), K(13), A(14)
                and seed go from 0 to 3 -> H(0), D(1), C(2), S(3);
            2 - the card has been converted as v = c.value-1 + c.seed*13.

            The conversion is then perfomed assuming all the values V are in the intervals 13s*[1, 13], with s representing the seed.
            1 - decrementing the current value by 1 to exploit the module operator % and so to return an interval from [0, 12] from
              the values, and inrementing by 2 to return in [2, 14];
            2 - for the seed is sufficient then, from the decreased values, to keep the integer quotient of v 
        */
        card mapValue2Card(short s);

        // Get deck 
        inline std::vector<card> getDeck() {return deck;}

        // Seed mapping to the corresping symbol (0 to Heart - 1 to Diamonds - 2 to Club - 3 to Spade)
        std::string mapSymbol(const int s);

        // Cards structure ordered according to its values
        std::vector<card> orderCards(std::vector<card> uCards);

        // Print cards list in a row
        void printList(std::vector<card> cardList);

        // Print the single card
        void printCard (card card);


        /*
        Return an ordered array in NumCPP format from the hand and cards vector represending the board
        */
        void getOrderedArray(const hand handSrc, const std::vector<card> boardSrc, std::vector<short> &v);

    } // namespace utility



    // ------------------------------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------------------------------



    namespace {     // hidden utilities group
    

        // ------------ FUNCTIONS ------------ 

        // Remove all elements of a specified value from a vector
        std::vector<card> remove(std::vector<card> cards, int val) {
            std::vector<card> cTemp;
            for (size_t i(0); i < cards.size(); i++) {
                if (cards[i].value != val) cTemp.push_back(cards[i]);                    
            }
            return cTemp;
        }

        // check if the card element is included in a card vector
        bool include(card c, std::vector<card> cList) {
/*
            utility::printCard(c);
            std::cout << "vs\t";
            utility::printList(cList);
            std::cout << "\n";*/

            for (card i:cList)
                if ((c == i)) return true;
            return false;            
        }

    
    }   // namespace -> end hidden utilities group


    // ------------------------------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------------------------------


    namespace evaluate {    // hands evaluation functions

        // Hand Evaluation Functions

        // One Pair Check - 10 iterations (2 loops)
        bool isOnePair(const std::vector<card> cards, rank &hRank, int &pairVal);
    
        // Two Pair Check - 11 iterations (2 loops)
        bool isTwoPair(const std::vector<card> cards, rank &hRank);   

        // Trips check - 8 iterations (2 loops)
        bool isTrips(const std::vector<card> cards, rank &hRank, int &trisVal);

        // Stairs check - 7 iterations (1 loop)
        bool isStraight(const std::vector<card> cards, rank &hRank);

        // Flush Check - 27 iterations (2-1 loops)
        bool isFlush(const std::vector<card> cards, rank &hRank);

        // Full House check - 18 iterations (2 callbacks)
        bool isFullHouse(const std::vector<card> cards, rank &hRank);

        // Quads check - 4 iterations (.5 loop)
        bool isQuad(const std::vector<card> cards, rank &hRank);

        // Straight Flush check - 14 iterations (1 loop and 1  callback)
        bool isStraightFlush(const std::vector<card> cards, rank &hRank);

        // Overall Iteration (worst case) -> 10 + 11 + 8 + 7+ 27 + 18 + 4 + 16 = 101 iterations



        //Combine all the cards, from hand and board, and evaluate returning the rank which consist in 3 values
            //- hand value ranging from 0 to 8 according to hand strength
            //- kickers values mapped with a base 15 encoding according to the kickers for each specific hand (e.g. pokers kickers are different from two pairs kickers) 
            //- an overall values that indicate the hand strength according to the hand "power" and kickers values encoded with a base 15
        rank rankHand(const hand handSrc, const std::vector<card> boardSrc, bool printBoard = false);       // TOTAL = 106 Iterations



        // Overload function to deal board cards and combination that report all the cards in a list ranging from 1 to 52
            // As first step evaluates that all the cards in the combination list is not already present in the board
            // if the check succed then the evaluation is performed in the same way of the previous function,
            // if the rank is present in the argument function and it return false if a card of the combination is in the board and true otherwise
        bool rankHand(size_t n, const short combList[], const std::vector<card> boardSrc, hand h, rank &hRank, bool printBoard = false);      // TOTAL = 106 Iterations

    } // namespace evaluate


    // ------------------------------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------------------------------


    namespace simulate {
        
        // Simulation Functions

        std::vector<hand> getPlayersHands(std::vector<card> &deckSrc, const int count);


        // Generate n different card of type poker::card to simulate the card in the poker board 
        void getBoard(std::vector<poker::card> &deckSrc, std::vector<poker::card> &board, int cBoard);


        // Evaluate the winner hands according to their overall computed values
        std::vector<int> getWinner(std::vector<rank> handRanks);


        // Function similar to getWinner() used if there are only two player which return 1 if the first rank is winning, 1/2 if tie and 0 otherwise
        float isWinner(rank p1, rank p2);

    } // namespace simulate    

} // namespace poker

#endif