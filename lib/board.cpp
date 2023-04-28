
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
    board::board(int num_player, bool save_result) : NUM_PLAYER{num_player}, 
                                                     COMBS(num_player), 
                                                     flag_save{save_result} {}

    board::~board() {}

    void board::init()
    {   
        std::cout << "----- BOARD INITIALIZATION -----\n";
        // removing all elements to ensure board is clear
        this->BOARD.clear();
        // reset phase turn and init a new deck
        this->PHASE = 0;
        this->DECK.restore();
        // reset pot parameters
        this->TOTAL_POT = 0;
        this->CURRENT_POT = 0;
        this->rounding_bet = 0;
        // initialize SB player position
        srand(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        this->position_offset = rand()%this->NUM_PLAYER;

        if (this->first_init)
        {
            this->PLAYERS_HAND.clear();
        // draw a new hand for each player
            for (size_t player_index = 0; player_index < NUM_PLAYER; player_index++)
            {
                PLAYERS_HAND.push_back(player(DECK.draw_hand(), &this->rounding_bet, &this->CURRENT_POT));
                if (flag_save)
                    PLAYERS_HAND[player_index].set_blind_index(COMBS.find_combinations(PLAYERS_HAND[player_index].get_sorted_position()));
                PLAYERS_HAND[player_index].set_board(&this->BOARD);
            }
            this->first_init = false;
        }
        else
        {
            // draw a new hand for each player
            for (size_t player_index = 0; player_index < NUM_PLAYER; player_index++)
            {
                PLAYERS_HAND[player_index].new_hand(DECK.draw_hand());
            }
        }
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
            for (auto &player:this->PLAYERS_HAND)
                player.set_board(&this->BOARD);
        }
        // increment turn counter
        this->PHASE++;

        if (flag_save && this->PHASE==FLOP)
            for (auto &player:PLAYERS_HAND)
                player.set_flop_index(COMBS.find_combinations(player.get_sorted_position()));

        return true;
    }

    int board::select_action(int player_index)
    {
        if (!this->PLAYERS_HAND[player_index].player_active())
        {
            std::cerr << "\nPlayer " << this->PLAYERS_HAND[player_index].ID << ": is not active!\n";
            return NO_ACTION;
        }
        int action, b;
        bool act = true;
        while (act)
        {
            try
            {   
                act = false;
                std::cout << "\nPlayer " << this->PLAYERS_HAND[player_index].ID << ": chose your action...\n";
                std::cin >> action;
                switch (action)
                {
                    case CHECK:                    
                        this->PLAYERS_HAND[player_index].check();
                        return CHECK;
                    case CALL:
                        this->PLAYERS_HAND[player_index].call();
                        return CALL;
                    case BET:
                        std::cout << "Amount to bet: ";
                        std::cin >> b;
                        this->PLAYERS_HAND[player_index].bet(b);
                        return BET;
                    case RAISE:
                        std::cout << "Amount to raise: ";
                        std::cin >> b;
                        this->PLAYERS_HAND[player_index].raise(b);
                        return RAISE;
                    case FOLD:
                        this->PLAYERS_HAND[player_index].fold();
                        return FOLD;
                    case ALLIN:
                        this->PLAYERS_HAND[player_index].allin();
                        return ALLIN;
                    case PRINT:
                        std::cout << *this;
                        return NO_ACTION;
                    default:
                        throw std::runtime_error("\n\tERROR: illegal action detected!\n");
                        break;
                }
            }
            catch(...)
            {
                std::cerr << "\n\tERROR: ILLEGAL ACTION PROVIDED" << '\n';
                act = true;
            }
        }
        return NO_ACTION;
    }

    bool board::play_turn()
    {
        this->TOTAL_POT = 0;
        this->CURRENT_POT = 0;
        
        this->init();
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
std::cout << "Stack: " << this->PLAYERS_HAND[player_index].get_stack()  << "\n";
            // if the player is not active (after a fold) is not required to take any action
            if (!this->PLAYERS_HAND[player_index].player_active() || this->PLAYERS_HAND[player_index].get_stack() == 0)
            {
                counter--;
                player_index = (player_index+1)%this->NUM_PLAYER;
            }
            else
            {   
                // take action
                int action = this->select_action(player_index);
                // evaluate if rounding bet is increased
                if (action == BET || action == RAISE || action == ALLIN)
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
        // after all player has played their turn update board parameters
        this->TOTAL_POT += this->CURRENT_POT;
        this->CURRENT_POT = 0;          // reset phase pot before drwing new board cards
        this->rounding_bet = 0;         // reset rounding bet for a new phase

        // resetting player current hand
        for (auto &player:PLAYERS_HAND)
            player.reset_current_bet();
        
        // if raise or bet UTG is the first on the last raise left
        if (PLAYERS_HAND[player_index].get_last_action() == BET || PLAYERS_HAND[player_index].get_last_action() == RAISE)
            player_index = (player_index+1)%this->NUM_PLAYER;

        std::cout << "--- DRAWING FLOP ---\n";        
        this->draw_board();    

        // reset counter for new phase
        counter = this->NUM_PLAYER;

        // iterate until all players reach the rounding bet
        while (counter > 0)
        {
std::cout << "Stack: " << this->PLAYERS_HAND[player_index].get_stack()  << "\n";
            // if the player is not active (after a fold) is not required to take any action
            if (!this->PLAYERS_HAND[player_index].player_active() || this->PLAYERS_HAND[player_index].get_stack() == 0)
            {
                counter--;
                player_index = (player_index+1)%this->NUM_PLAYER;
            }
            else
            {   
                // take action
                int action = this->select_action(player_index);
                // evaluate if rounding bet is increased
                if (action == BET || action == RAISE || action == ALLIN)
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
        // after all player has played their turn update board parameters
        this->TOTAL_POT += this->CURRENT_POT;
        this->CURRENT_POT = 0;          // reset phase pot before drwing new board cards
        this->rounding_bet = 0;         // reset rounding bet for a new phase
        // resetting player current hand
        for (auto &player:PLAYERS_HAND)
            player.reset_current_bet();
        
        // if raise or bet UTG is the first on the last raise left
        if (PLAYERS_HAND[player_index].get_last_action() == BET || PLAYERS_HAND[player_index].get_last_action() == RAISE)
            player_index = (player_index+1)%this->NUM_PLAYER;

        std::cout << "--- DRAWING TURN ---\n";        
        this->draw_board();    

        // reset counter for new phase
        counter = this->NUM_PLAYER;

        // iterate until all players reach the rounding bet
        while (counter > 0)
        {
std::cout << "Stack: " << this->PLAYERS_HAND[player_index].get_stack()  << "\n";
            // if the player is not active (after a fold) is not required to take any action
            if (!this->PLAYERS_HAND[player_index].player_active() || this->PLAYERS_HAND[player_index].get_stack() == 0)
            {
                counter--;
                player_index = (player_index+1)%this->NUM_PLAYER;
            }
            else
            {   
                // take action
                int action = this->select_action(player_index);
                // evaluate if rounding bet is increased
                if (action == BET || action == RAISE || action == ALLIN)
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
        // after all player has played their turn update board parameters
        this->TOTAL_POT += this->CURRENT_POT;
        this->CURRENT_POT = 0;          // reset phase pot before drwing new board cards
        this->rounding_bet = 0;         // reset rounding bet for a new phase
        // resetting player current hand
        for (auto &player:PLAYERS_HAND)
            player.reset_current_bet();
        
        // if raise or bet UTG is the first on the last raise left
        if (PLAYERS_HAND[player_index].get_last_action() == BET || PLAYERS_HAND[player_index].get_last_action() == RAISE)
            player_index = (player_index+1)%this->NUM_PLAYER;

        std::cout << "--- DRAWING RIVER ---\n";        
        this->draw_board();    

        // reset counter for new phase
        counter = this->NUM_PLAYER;

        // iterate until all players reach the rounding bet
        while (counter > 0)
        {
std::cout << "Stack: " << this->PLAYERS_HAND[player_index].get_stack()  << "\n";
            // if the player is not active (after a fold) is not required to take any action
            if (!this->PLAYERS_HAND[player_index].player_active() || this->PLAYERS_HAND[player_index].get_stack() == 0)
            {
                counter--;
                player_index = (player_index+1)%this->NUM_PLAYER;
            }
            else
            {   
                // take action
                int action = this->select_action(player_index);
                // evaluate if rounding bet is increased
                if (action == BET || action == RAISE || action == ALLIN)
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
        // after all player has played their turn update board parameters
        this->TOTAL_POT += this->CURRENT_POT;
        this->CURRENT_POT = 0;          // reset phase pot before drwing new board cards
        this->rounding_bet = 0;         // reset rounding bet for a new phase
        // resetting player current hand
        for (auto &player:PLAYERS_HAND)
            player.reset_current_bet();

        std::cout << "---- HAND EVALUATION ----\n";
        for (auto &player:this->PLAYERS_HAND)
            player.rank_hand();

        std::vector<player*> winners = this->get_winner();
    
        std::cout << "A total of " << winners.size() << " winners found!\n";

        for (auto player:winners)
            player->pay_win(this->TOTAL_POT/static_cast<float>(winners.size()));
        
        this->TOTAL_POT=0;

        std::cout << *this;

        return true;
    }


    std::vector<player*> board::get_winner() 
    {
        std::vector<player*> winners;               // initialize winners list
        int max_Rank = 0;                           // initialize max rank backup

        for (auto &player:this->PLAYERS_HAND)
        {
            if (player.get_rank().OverallValue > max_Rank)              // If an highest values is found
            {
                winners.clear();                                        // Clear the winners vector
                winners.push_back(&player);                             // Add the winner position
                max_Rank = player.get_rank().OverallValue;              // Update the max value for the next control
            } else if (player.get_rank().OverallValue > max_Rank)       // If the current hand value is the same of the current max value
                winners.push_back(&player);                             // Adding the current position to the winners list
        }
        return winners;
    }

} // namespace poker

