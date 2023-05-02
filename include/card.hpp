/**
 * @file card.hpp
 * @author Mattia Scarpa (sct.mtt.25@gmail.com)
 * @brief Card class structure function & member declaration
 * @version 0.1
 * @date 2023-03-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

constexpr int MIN_VAL{2};
constexpr int MAX_VAL{14};
constexpr int MIN_SEED{0};
constexpr int MAX_SEED{3};


namespace poker {


    /**
     * @brief data structure to define a generic card
     * 
     */
    struct card_str
        {
            int value, seed = 0;   // Value-Seed declaration, they fully define a single card.
        };

    /**
     * @brief operator to print symbolic card from struct
     * 
     * @return std::ostream& output stream
     */
    std::ostream& operator<<(std::ostream& os, const card_str& c_str);

    /**
     * @brief Class containing card values sorted by its deck position or its seed-value combination
     * To define its convention we assume a 52 cards deck, 4 seeds, witch values sorted from 2-14 where 14 
     * represent Aces, which typically takes the highest values, with the exception of lower straight.
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
    class card
    {

    private:

    card_str c;        

    public:

        /**
         * @brief Construct a new card object, passed as value & seed.
         *          Note: value is typically from 1 to 13, however to better map aces we must increase the value by 1 
         *          so to consider ace value become 14 and the other remain from 2 to 13
         * 
         * @param v card value (1 to 13) then increased by 1 (2 to Ace(14))
         * @param s card seed (1 to 4) then decreased by 1
         */
        card(int v, int s);

        /**
         * @brief Construct a new card object, passed as structure.
         * 
         * @param c 
         */
        card(card_str c);

        /**
         * @brief Construct a new card object, passed as deck position
         * 
         * @param p card deck position (from 1 to 52)
         */
        card(short p);

        /**
         * @brief Removing the default constructor
         * 
         */
        card() = delete;

        /**
         * @brief Destroy the card object
         * 
         */
        ~card();


        // Functions

        /**
         * @brief Get the card deck position 
         * 
         * @return int card position mapped from 1 to 52 with the current convention
         */
        inline int get_position() const {return c.seed*13 + c.value-1;}

        /**
         * @brief Get the card structure
         * 
         * @return card_str card structure containing card information
         */
        inline card_str get_card() const {return c;}


        /**
         * @brief operator to print symbolic card
         *
         * @return std::ostream& output stream
         */
        friend std::ostream& operator<<(std::ostream& os, const card& card_obj)
        {
            std::string sym;
            // map symbols according to card seed
            switch (card_obj.c.seed)
            {
            case 0:
                sym = "\xE2\x99\xA5";
                break;
            case 1:
                sym = "\xE2\x99\xA6";
                break;
            case 2:
                sym = "\xE2\x99\xA3";
                break;
            case 3:
                sym = "\xE2\x99\xA0";
                break;
            default:
                throw std::invalid_argument("Error in seed conversion!");
                break;
            }

            // if MAX_VAL is 14 then print ACE
            if (card_obj.c.value == 14)
            {
                os << 'A' << sym << " ";
                return os;
            }
            
            os << card_obj.c.value << sym << " ";
            return os;
        }

        /**
         * @brief equal operator overload
         * 
         */
        inline bool operator==(const card& c) const {
            return this->get_position() == c.get_position();
        }
        /**
         * @brief smaller operator overload
         * 
         */
        inline bool operator<(const card& c) {
            return this->get_card().value < c.get_card().value;
        }
        /**
         * @brief greater operator overload
         * 
         */
        inline bool operator>(const card& c) {
            return this->get_card().value > c.get_card().value;
        }
        /**
         * @brief smaller or equal operator overload
         * 
         */
        inline bool operator<=(const card& c) {
            return this->get_card().value <= c.get_card().value;
        }
        /**
         * @brief greater or equal operator overload
         * 
         */
        inline bool operator>=(const card& c) {
            return this->get_card().value >= c.get_card().value;
        }
    };
} // namespace poker