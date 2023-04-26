/**
 * @file deck.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <memory>
#include <chrono>
#include <algorithm>
#include <vector>
#include <math.h>

#include "../include/card.hpp"
#include "../include/hand.hpp"
#include "../include/utilities.hpp"

#include "../include/deck.hpp"

namespace poker
{
    /***************************************************************************************
     ***************************************************************************************
     ************************************ CONSTRUCTORS *************************************
     ***************************************************************************************
     ***************************************************************************************/

    // lambda function initialization
    deck::deck() : Deck([](){
        std::vector<card> c;
        for (size_t i = 1; i <= 52; i++)
            c.push_back(card(i));
        return c;
    }()), Deck_backup{Deck} // constant backup deck for faster restore
    {}

    deck::~deck() {}

    /***************************************************************************************
     ***************************************************************************************
     ************************************** FUNCTIONS **************************************
     ***************************************************************************************
     ***************************************************************************************/

    card deck::draw() 
    {
        // initialize rand seed
        srand(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        return utility::remove_at(Deck, rand()%Deck.size());
    }

    std::vector<card> deck::draw(int n) 
    {
        std::vector<card> c;
        // repeat draw n times
        for (size_t i = 0; i < n; i++)
            c.push_back(draw());
        return c;        
    }

    // COMPILATION ERROR... BUG UNKNOWN
    hand deck::draw_hand() 
    {
        return hand(draw(), draw());
    }
    
} // namespace poker
