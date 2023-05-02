/**
 * @file hand.hpp
 * @author Mattia Scarpa (sct.mtt.25@gmail.com)
 * @brief Hand class structure function & member declaration
 * @version 0.1
 * @date 2023-04-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <iterator>
#include <algorithm>

#include "../include/card.hpp"

namespace poker
{
    /**
     * @brief rank data structure used in hand class to define the strength of a card according to current board.
     * NOTE: rank can be defined only if a valid board is provided
     *  
     */
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
    /**
     * @brief hand class that keep 2 cards information
     * 
     */
    class hand
    {
    private:
        /***************************************************************************************
         ***************************************************************************************
         ************************************** MEMBERS ****************************************
         ***************************************************************************************
         ***************************************************************************************/

        /**
         * @brief hand vector contining 2 card (inserted in constructor)
         * 
         */
        std::vector<card> Hand;
        /**
         * @brief pointer to vector representing board
         * 
         */
        std::vector<card>* Board = nullptr;
        bool Board_valid = false;   // initially no board is provided

        /**
         * @brief vector containing all hand and board card ordered, initialized only during rank_hand()
         * 
         */
        std::vector<card> Ordered_Card_List{};

        /**
         * @brief structure of values to define "how good" is the current hand. 
         */
        rank Rank;

        /**
         * @brief Order cards according to its value (2 to 14)
         * 
         * @return std::vector<card> 
         */
        std::vector<card> order_cards();

        /**
         * @brief Remove all elements of a specified value from a vector
         * 
         * @param val value to remove
         * @return std::vector<card> cards vector without the val
         */
        std::vector<card> remove(int val) {
            std::vector<card> cTemp;
            for (size_t i(0); i < Ordered_Card_List.size(); i++) {
                if (Ordered_Card_List[i].get_card().value != val) cTemp.push_back(Ordered_Card_List[i]);                    
            }
            return cTemp;
        }

        /**
         * @brief compare function for sort operation
         * 
         * @param c1 first card object
         * @param c2 second card object
         * @return true if c1 < c2
         * @return false 
         */
        static bool compare_position(const card c1, const card c2) 
        {
            return c1.get_position() <= c2.get_position();
        }

        // Hand Evaluation Functions

        /**
         * @brief evaluate Hand and Board to check if is one Pair, it ranks then according to the highest pair value and kickers
         * 
         * @param pairVal highest pair values found
         * @return true if a single pair is found
         * @return false if no pair is found
         */
        bool isOnePair(int &pairVal);

        /**
         * @brief One pair overload function
         * 
         * @param Card_List provided ordered card list to evaluate
         * @param pairVal highest pair values found
         * @return true if a single pair is found
         * @return false if no pair is found
         */
        bool isOnePair(std::vector<card> Card_List, int &pairVal);
    
        /**
         * @brief evaluate Hand and Board to check if is two Pair, it ranks then according to the highest pairs values and kicker
         * 
         * @return true if a two pair is found
         * @return false if no two pair is found
         */
        bool isTwoPair();   

        /**
         * @brief evaluate Hand and Board to check if is trips, it ranks then according to trips value and kickers
         * 
         * @return true if a trips is found
         * @return false if no trips pair is found
         */
        bool isTrips(int &trisVal);

        /**
         * @brief evaluate Hand and Board to check if is straight, it ranks then according to highest straight value
         * 
         * @return true if a straight is found
         * @return false if no straight pair is found
         */
        bool isStraight();

        /**
         * @brief Straight overload function
         * 
         * @param Card_List provided ordered card list
         * @return true if a straight is found
         * @return false if no straight pair is found
         */
        bool isStraight(std::vector<card> Card_List);

        /**
         * @brief evaluate Hand and Board to check if is flush, it ranks then according to highest flush card value
         * 
         * @return true if a flush is found
         * @return false if no flush pair is found
         */
        bool isFlush();

        /**
         * @brief evaluate Hand and Board to check if is full house, it ranks then according to trips and pair values
         * 
         * @return true if a full house is found
         * @return false if no full house pair is found
         */
        bool isFullHouse();

        /**
         * @brief evaluate Hand and Board to check if is quads, it ranks then according to quads value and kicker
         * 
         * @return true if a quads is found
         * @return false if no quads pair is found
         */
        bool isQuad();

        /**
         * @brief evaluate Hand and Board to check if is straight flush, it ranks then according to highest straight card value
         * 
         * @return true if a straight flush is found
         * @return false if no straight flush pair is found
         */
        bool isStraightFlush();

    public:

        /***************************************************************************************
         ***************************************************************************************
         ************************************ CONSTRUCTORS *************************************
         ***************************************************************************************
         ***************************************************************************************/

        /**
         * @brief Construct a new hand object
         * 
         * @param c1 first card of type card
         * @param c2 second card of type card
         */
        hand(card c1, card c2);

        /**
         * @brief Construct a new hand object
         * 
         * @param v card value from 1 to 52
         */
        hand(short v1, short v2);

        /**
         * @brief Remove default contructor (a card mst be initialized with a legal value)
         * 
         */
        hand() = delete;

        /**
         * @brief Destroy the hand object
         * 
         */
        ~hand();


        /***************************************************************************************
         ***************************************************************************************
         ************************************** FUNCTIONS **************************************
         ***************************************************************************************
         ***************************************************************************************/

        inline std::vector<card> get_hand()
        {
            return Hand;
        }
        
        /**
         * @brief Set the board passing the board vector pointer. It must be of size 3,4,5
         * 
         * @param board board input vector pointer
         */
        bool set_board(std::vector<card>* board);

        /**
         * @brief run hand ranking according to current hand and board provided
         * 
         * @return true if the evaluation complete succesfuly
         * @return false if board is missing
         */
        bool rank_hand();

        /**
         * @brief return if a valid board i set
         *
         */
        inline bool board_valid() {return Board_valid;}

        /**
         * @brief Remove the board reference pointing to nullptr
         * 
         */
        inline void clear_board() 
        {   
            Board = nullptr;
            Board_valid = false;
        }

        /**
         * @brief Set new cards pair for the hand, clear board by default (this option can be disabled)
         * 
         * @param c1 first card object
         * @param c2 second card object
         * @param clear bolean to choose if clear board
         * @return true if new hand is set
         */
        inline bool new_hand(card c1, card c2, bool clear = true)
        {
            Hand = {c1, c2};
            if (clear)
                clear_board();
            return true;
        }

        /**
         * @brief Set new cards pair for the hand, clear board by default (this option can be disabled)
         * 
         * @param c1 first card deck position
         * @param c2 second card deck position
         * @param clear bolean to choose if clear board
         * @return true if new hand is set
         */
        inline bool new_hand(int v1, int v2, bool clear = true)
        {
            Hand = {card(v1), card(v2)};
            if (clear)
                clear_board();
            return true;
        }

        /**
         * @brief Get the card vector sorted by its dec position
         * 
         * @return std::vector<card> 
         */
        std::vector<short> get_sorted_position()
        {   
            // merge hand and board vector
            std::vector<card> card_list;
            if (Board_valid)
            {
                card_list.reserve(Hand.size()+Board->size());
                std::merge(Hand.begin(), Hand.end(), Board->begin(), Board->end(), std::back_inserter(card_list));
            }
            else
            {
                card_list = Hand;
            }            

            // sort of resulting merged vector

            std::vector<short> sorted_positions;
            std::transform(card_list.begin(), card_list.end(), std::back_inserter(sorted_positions),
                   [](const card& c) { return c.get_position(); });
            std::sort(sorted_positions.begin(), sorted_positions.end());
            return sorted_positions;
        }

        /**
         * @brief Get the hand rank
         * 
         * @return rank structure
         */
        rank get_rank() {return Rank;}

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
        friend std::ostream& operator<<(std::ostream& os, const hand& h) 
        {   
            os << "RANK\t| HAND CARDS\t| CURRENT BOARD \t"
               << "\n-----------------------------------------------\n";
            
            os << " " << h.Rank.value << "\t|";

            for (card c : h.Hand)
                os << c;
            os << "  \t| ";
            if (h.Board != nullptr)
                for (card c : *h.Board)
                    os << c;

            os << "\n";
            return os;
        }
    };
    
} // namespace poker

