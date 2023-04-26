#pragma once
#include "../include/hand.hpp"

namespace poker
{
    /**
     * @brief deck class contining all 52 cards - supports draw hand and a single card functions
     * 
     */
    class deck
    {   

    private:

        std::vector<card> Deck;
        const std::vector<card> Deck_backup;

    public: 

        /***************************************************************************************
         ***************************************************************************************
         ************************************ CONSTRUCTORS *************************************
         ***************************************************************************************
         ***************************************************************************************/

        /**
         * @brief Construct a new deck object according to card convention defined as follows.
         *      
         * Our card deck position can be showed in a table as:
         *  
         *          Values  |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 | 11 | 12 | 13 | 14 |
         * -----------------------------------------------------------------------------------
         *  Seeds: Hearts   |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 | 11 | 12 | 13 |
         *         Diamonds | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 |
         *         Clubs    | 27 | 28 | 29 | 30 | 31 | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 |
         *         Spades   | 40 | 41 | 42 | 43 | 44 | 45 | 46 | 47 | 48 | 49 | 50 | 51 | 52 |
         * 
         */
        deck();

        /**
         * @brief Destroy the deck object
         * 
         */
        ~deck();



        /***************************************************************************************
         ***************************************************************************************
         ************************************** FUNCTIONS **************************************
         ***************************************************************************************
         ***************************************************************************************/
        
        /**
         * @brief draw a single card from the available deck
         * 
         * @return card object extracted randomly from deck
         */
        card draw();

        /**
         * @brief draw n card from the available deck
         * 
         * @param n number of card to draw
         * @return std::vector<card> n card extracted
         */
        std::vector<card> draw(int n);

        /**
         * @brief draw exactly 2 cards to build a hand object
         * 
         * @return hand object
         */
        hand draw_hand();
        //poker::hand draw_hand();
        //error: ‘hand’ does not name a type

        /**
         * @brief restore the original 52 cards deck 
         * 
         */
        inline void restore()
        {
            Deck = Deck_backup;
        }


        /***************************************************************************************
         ***************************************************************************************
         ************************************** OPERATORS **************************************
         ***************************************************************************************
         ***************************************************************************************/
        
        /**
         * @brief print sequence of symbolic cards
         * 
         * @return std::ostream& output stream
         */
        friend std::ostream& operator<<(std::ostream& os, const deck& d) 
        {   
            for (card c : d.Deck)
                os << c;
            return os;
        }
    };
    
} // namespace poker

