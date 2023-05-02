/**
 * @file card.cpp
 * @author Mattia Scarpa (sct.mtt.25@gmail.com)
 * @brief implementation defining card class functions 
 * @version 0.1
 * @date 2023-03-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <ostream>
#include <string>
#include <assert.h>

#include "../include/card.hpp"


namespace poker {

    card::card(int v, int s): c{v+1, s}
    {   
        if (!(this->c.value <= MAX_VAL & this->c.value >= MIN_VAL))
            throw std::invalid_argument("Invalid Value!!!");
        if (!(this->c.seed <= MAX_SEED & this->c.seed >= MIN_SEED))
            throw std::invalid_argument("Invalid seed!!!");
    }


    card::card(card_str c) 
    {
        this->c = c;
        if (!(this->c.value <= MAX_VAL & this->c.value > 1))
            throw std::invalid_argument("Invalid Value!!!");
        if (!(this->c.seed < MAX_SEED & this->c.seed >= 0))
            throw std::invalid_argument("Invalid seed!!!");
        
    }

    card::card(short p)
    {
        assert(1 <= p && p <= 52 && "Value must be included in the set [1, 52]");
        p--; // remapping the value from [1, 52] to [0, 51] to make the values easier to map 

        c.seed = p/13;      // getting seed
        c.value = p%13+2;   // getting value (0-12 -> 2-14)
            // value conversion preserve the convention assigning to Aces the highest position
    }

    card::~card(){}

    std::ostream& operator<<(std::ostream& os, const card_str& c_str) 
    {
            std::string sym;

            // map symbols according to card seed
            switch (c_str.seed)
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
            if (c_str.value == 14)
            {
                os << 'A' << sym << " ";
                return os;
            }
            
            os << c_str.value << sym << " ";
            return os;
    }

} // namespace poker