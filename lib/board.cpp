
#include <iostream>
#include <vector>
#include <algorithm>

#include "../include/combinations.hpp"
#include "../include/card.hpp"
#include "../include/deck.hpp"
#include "../include/hand.hpp"
#include "../include/player.hpp"
#include "../include/board.hpp"



namespace poker
{
    board::board(int num_player, bool save_result) : NUM_PLAYER{num_player}, COMBS(num_player), flag_save{save_result} {}

    board::~board() {}

    void board::init()
    {   
        std::cout << "----- BOARD INITIALIZATION -----\n";
        // removing all elements to ensure board is clear
        this->BOARD.clear();
        // reset phase turn and init a new deck
        this->PHASE = 0;
        this->DECK.restore();
        // reset pot
        this->TOTAL_POT = 0;
        this->CURRENT_POT = 0;
        // initialize SB player position
        srand(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        this->position_offset = rand()%this->NUM_PLAYER;

        if (this->first_init)
        {
            this->PLAYERS_HAND.clear();
        // draw a new hand for each player
            for (size_t i = 0; i < NUM_PLAYER; i++)
            {
                PLAYERS_HAND.push_back(player(DECK.draw_hand(), &this->rounding_bet, &this->CURRENT_POT));
                PLAYERS_HAND[i].set_board(&this->BOARD);
            }
            this->first_init = false;
        }
        else
        {
            // draw a new hand for each player
            for (size_t i = 0; i < NUM_PLAYER; i++)
            {
                PLAYERS_HAND[i].new_hand(DECK.draw_hand());
            }
        }
        if (flag_save)
            for (auto player:PLAYERS_HAND)
                player.set_blind_index(COMBS.find_combinations(player.get_sorted_position()));
    }

    bool board::draw_board()
    {   
        if (this->PLAYERS_HAND.empty()) 
        {
            std::cerr << "ERROR: NO PLAYERS FOUND IN BOARD!\n";
            exit(1);
        }
        if (this->PHASE >= RIVER) 
        {
            std::cerr << "MAX TURN REACHED, RESET BOARD WITH init() TO START A NEW GAME!\n";
            return false;
        }
        // draw n cards according to current turn
        std::vector<card> card_temp{this->DECK.draw(1+(this->PHASE==BLIND)*2)};
        // adding the new cards drawn in board
        BOARD.insert(this->BOARD.end(), card_temp.begin(), card_temp.end());
        // providing board to each hand
        if (!this->PLAYERS_HAND.empty() & !this->PLAYERS_HAND[0].board_valid()) 
        {
            for (auto player:this->PLAYERS_HAND)
                player.set_board(&this->BOARD);
        }
        // increment turn counter
        this->PHASE++;

        if (flag_save && this->PHASE==BLIND)
            for (auto player:PLAYERS_HAND)
                player.set_blind_index(COMBS.find_combinations(player.get_sorted_position()));


        return true;
    }

    bool board::play_turn()
    {
        this->TOTAL_POT = 0;
        this->CURRENT_POT = 0;
        
        this->init();
    std::cout << *this << "\n";
        std::cout << "-------------- BLIND --------------\n";

        // ensuring valid player position
        this->PLAYERS_HAND[(this->position_offset)%this->NUM_PLAYER].bet(SB);
        this->PLAYERS_HAND[(this->position_offset+1)%this->NUM_PLAYER].raise(BB);

        // selecting utg player
        int player_index = (this->position_offset+2)%this->NUM_PLAYER;
        // initialize a counter of player that has to 
        int counter = this->NUM_PLAYER;

        // iterate until all players reach the rounding bet
        while (counter > 0)
        {
            // if the player is not active (after a fold) is not required to take any action
            if (!this->PLAYERS_HAND[player_index].player_active())
            {
                counter--;
                player_index = (player_index+1)%this->NUM_PLAYER;
            }
            else
            {   
                // take action
                int action = this->select_action(player_index);
                // evaluate if rounding bet is increased
                if (action == BET || action == RAISE)
                {
                    counter = this->NUM_PLAYER-1;
                    player_index = (player_index+1)%this->NUM_PLAYER;
                }
                else if (action != NO_ACTION)
                {
                    counter--;
                    player_index = (player_index+1)%this->NUM_PLAYER;
                }
            }
            std::cout << "COUNTER: " << counter << "\n";
        }
        // after all player has played their turn update board parameters
        this->TOTAL_POT += this->CURRENT_POT;
        this->CURRENT_POT = 0;          // reset phase pot before drwing new board cards
        this->rounding_bet = 0;         // reset rounding bet for a new phase

    std::cout << *this << "\n";

        for (auto &player:PLAYERS_HAND)
            player.reset_current_bet();

        std::cout << "--- DRAWING FLOP ---\n";        
        this->draw_board();    

    std::cout << *this << "\n";
        // reset counter for new phase
        counter = this->NUM_PLAYER;

        // iterate until all players reach the rounding bet
        while (counter > 0)
        {
            // if the player is not active (after a fold) is not required to take any action
            if (!this->PLAYERS_HAND[player_index].player_active())
            {
                counter--;
                player_index = (player_index+1)%this->NUM_PLAYER;
            }
            else
            {   
                // take action
                int action = this->select_action(player_index);
                // evaluate if rounding bet is increased
                if (action == BET || action == RAISE)
                {
                    counter = this->NUM_PLAYER-1;
                    player_index = (player_index+1)%this->NUM_PLAYER;
                }
                else if (action != NO_ACTION)
                {
                    counter--;
                    player_index = (player_index+1)%this->NUM_PLAYER;
                }
            }
        }


        


        return true;
    }
} // namespace poker

