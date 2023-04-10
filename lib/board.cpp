
#include <iostream>
#include <vector>
#include <algorithm>

#include "../include/combinations.hpp"
#include "../include/card.hpp"
#include "../include/deck.hpp"
#include "../include/hand.hpp"
#include "../include/board.hpp"



namespace poker
{
    board::board(int num_player) : NUM_PLAYER(num_player) {}

    board::~board() {}

    void board::init()
    {   
        // removing all elements to ensure board is clear
        PLAYERS_HAND.clear();
        // draw a new hand for each
        for (size_t i = 0; i < NUM_PLAYER; i++)
            //PLAYERS_HAND.push_back(DECK.draw_hand());
    }

    bool board::draw_board()
    {   
        if (TURN > 2) 
        {
            std::cerr << "MAX TURN REACHED, RESET BOARD WITH reset() TO START A NEW GAME!\n";
            return false;
        }

        // draw n cards according to current turn
        std::vector<card> card_temp{DECK.draw(1+(TURN==0)*2)};
        BOARD.insert(BOARD.end(), card_temp.begin(), card_temp.end());
        // increment turn counter
        TURN++;
        return true;
    }
} // namespace poker

