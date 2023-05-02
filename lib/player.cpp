
#include <iostream>
#include <vector>
#include <stdexcept>


#include "../include/card.hpp"
#include "../include/hand.hpp"
#include "../include/player.hpp"

namespace poker
{
    player::player(card c1, card c2, int bb, float* r, float* p) : Hand(c1, c2), BB{bb}, rounding_bet{r}, current_pot{p} {}

    player::player(hand h, int bb, float* r, float* p) : Hand{h}, BB{bb}, rounding_bet{r}, current_pot{p} {}

    player::~player() {}

    bool player::check()
    {   
        // check if player is active
        if (!this->is_active)
        {
            std::cerr << "WARNING: player is not active\n";
            this->last_action = NO_ACTION;
            return false;
        }        
        std::cout << "Player " << this->ID << ": check the hand...";
        // evaluating if check is a legal move
        if (this->current_bet == *this->rounding_bet)
        {
            // set last action
            this->last_action = CHECK;
            this->last_bet = 0;
            std::cout << " succes!\n";
            return true;
        }
        // if illegal move throw a runtime exception
        else if (this->current_bet < *this->rounding_bet)
        {
            std::cerr << "ERROR: you are not allowed to check if rounding bet is higher than your current bet!\n";
            this->FLAG_ERROR = true;
            throw std::runtime_error("ERROR: you are not allowed to check if rounding bet is higher than your current bet!\n");
        }
        std::cerr << "WARNING: checking with an higher bet than the rounding bet should not happens!\n";
        return false;
    }

    bool player::call()
    {
        // check if player is active
        if (!this->is_active)
        {
            std::cerr << "WARNING: player is not active\n";
            this->last_action = NO_ACTION;
            return false;
        }
        std::cout << "Player " << this->ID << ": call the bet...";
        // evaluate feasibility of the action
        if (this->current_bet < *this->rounding_bet)
        {
            // find the new bet
            int call_val = *this->rounding_bet - this->current_bet;
            // update player situation
            this->current_bet += call_val;
            this->stack -= call_val;
            if (this->current_pot != nullptr)
                *this->current_pot += call_val;
            else
                std::cerr << "\nWARNING: no link to the pot provided, it must be managed from your board object!\n";
            // set last action
            this->last_action = CALL;
            this->last_bet = call_val;
            std::cout << " succes!\n";
            return true;
        }
        // check for illegal action
        // if bet is equal to rounding bet than correct action should be check rather than call
        if (this->current_bet == *this->rounding_bet)
        {
            std::cerr << "WARNING: call is not required when current bet is equal to rounding bet, cheking instead!\n";
            return this->check();
        }
        // if bet is higher than rounding bet throw runtime exception to report illegal move
        if (this->current_bet > *this->rounding_bet)
        {
            std::cerr << "ERROR: you should not call if your current bet is higher than rounding bet!\n";
            this->FLAG_ERROR = true;
            throw std::runtime_error("ERROR: you should not call if your current bet is higher than rounding bet!\n");
        }
        return false;        
    }

    bool player::bet(int n, bool is_preflop)
    {   
        // check if player is active
        if (!this->is_active)
        {
            std::cerr << "WARNING: player is not active\n";
            this->last_action = NO_ACTION;
            return false;
        }
        if (n < this->BB && !is_preflop)
        {
            std::cerr << "WARNING: you can not bet less than chosen BigBlind!\n";
            n = this->BB;
            this->FLAG_BET_WARNING = true;
        }
        
        std::cout << "Player " << this->ID << ": bet " << n << "...";
        // evaluate board situation if bet is allowed
        if (this->stack == 0)
        {
            std::cerr << "\nWARNING: you have no stack to place a bet!\n";
            this->last_action = NO_ACTION;
        }
        if (this->current_bet < *this->rounding_bet)
        {
            std::cerr << "\nWARNING: bet is not allowed in this condition, taking raise action!\n";
            return raise(n);
        }
        // evaluate if player has enough stack to place a bet
        if (n >= this->stack)
        {
            std::cerr << "\nWARNING: you can not bet more than your maximum stack, allin instead!\n";
            if (!is_preflop)
                return this->allin();
            return this->allin();
        }
        else if (this->current_bet == *this->rounding_bet)
        {   
            // otherwise update values
            this->stack -= n;
            this->current_bet += n;
            *this->current_pot += n;
            *this->rounding_bet += n;
            // set last action
            this->last_action = BET;
            this->last_bet = n;
            std::cout << " succes\n";
            return true;
        }
        else
        {
            this->FLAG_ERROR = true;
            throw std::runtime_error("ERROR: an undefined error occurred...\n");
        }
        return false;
    }

    bool player::raise(int n, bool is_preflop)
    {
        // check if player is active
        if (!this->is_active)
        {
            std::cerr << "WARNING: player is not active\n";
            this->last_action = NO_ACTION;
            return false;
        }
        if (n-*this->rounding_bet < this->BB && !is_preflop)
        {
            std::cerr << "WARNING: you can not raise less than BigBlind over the current rounding bet!\n";
            n = (this->BB + *this->rounding_bet) - this->current_bet;
            this->FLAG_BET_WARNING = true;
        }     
        std::cout << "Player " << this->ID << ": raise " << n << "...";
        // evaluate board situation if bet is allowed
        if (this->stack == 0)
        {
            std::cerr << "\nWARNING: you have no stack to place a bet!\n";
            this->last_action = NO_ACTION;
            return false;
        }
        if ((this->current_bet+n) < *this->rounding_bet)
            {
                std::cerr << "\nERROR: you are not raising enough!\n";
                this->FLAG_ERROR = true;
                throw std::runtime_error("\nERROR: you are not raising enough!\n");
            }
        else if ((this->current_bet+n) == *this->rounding_bet)
            {
                std::cerr << "\nWARNING: you are not supposed to equalise rounding bet raising, calling instead...\n";
                this->call();
            }
        else if (this->current_bet == *this->rounding_bet)
            {
                std::cerr << "\nWARNING: you are not supposed to raise if your current bet is equal to rounding bet, betting instead...\n";
                this->bet(n);
            }
        else if (this->current_bet < *this->rounding_bet)
        {
            // otherwise update values
            *this->rounding_bet = this->current_bet+n;
            *this->current_pot += n;
            this->stack -= n;
            this->current_bet += n;
            // set last action
            this->last_bet = n;
            this->last_action = RAISE;
            std::cout << " succes\n";
            return true;
        }
        return true;
    }

    bool player::fold()
    {
        // check if player is active
        if (!this->is_active)
        {
            std::cerr << "WARNING: player is not active\n";
            this->last_action = NO_ACTION;
            return false;
        }        
        std::cout << "Player " << this->ID << ": fold his hand...";
        if (this->current_bet == *this->rounding_bet)
        {
            std::cerr << "WARNING: you can not fold your hand if you equlise rounding bet, check instead...\n";
            return this->check();
        }
        this->is_active = false;
        this->last_bet = 0;
        this->last_action = FOLD;
        std::cout << " success!\n";
        return true;
    }

    bool player::allin()
    {
        // check if player is active
        if (!this->is_active)
        {
            std::cerr << "WARNING: player is not active\n";
            this->last_action = NO_ACTION;
            return false;
        }        
        std::cout << "Player " << this->ID << ": allin his hand...";
        *this->current_pot += this->stack;
        this->current_bet += this->stack;
        if (this->stack > *this->rounding_bet)
            *this->rounding_bet = this->stack; 
        this->last_bet = this->stack;
        this->last_action = ALLIN;
        this->stack = 0;
        std::cout << " succes!\n";      
        return true;  
    }
} // namespace poker
