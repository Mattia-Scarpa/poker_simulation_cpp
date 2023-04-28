#pragma once

#include <chrono>

namespace poker
{

    #define NO_ACTION -1
    #define CHECK 0
    #define CALL 1
    #define BET 2
    #define RAISE 3
    #define FOLD 4
    #define ALLIN 5
    #define PREFLOP 6

    #define PRINT 9

    class player
    {
    private:
        
        /**
         * @brief player hand
         * 
         */
        hand Hand;

        /**
         * @brief default player chips stack
         * 
         */
        float stack = 10000;
        
        /**
         * @brief rounding bet value, to keep playing rounding bet value must be at least equalized
         * 
         */
        float* rounding_bet = nullptr;

        /**
         * @brief pointer to phase pot
         * 
         */
        float* current_pot = nullptr;

        /**
         * @brief value to keep the last action taken by the player
         * 
         */
        int last_action = NO_ACTION;

        /**
         * @brief player status, a true value means an actve 
         * 
         */
        bool is_active = true;

        /**
         * @brief index in combination lists
         * 
         */
        int blind_index = -1;
        int flop_index = -1;

    public:

        long int ID{std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count()%10000};


        /**
         * @brief value for a specific phase bet used to compare with the rounding_bet 
         * 
         */
        float current_bet = 0;

        /***************************************************************************************
         ***************************************************************************************
         ************************************ CONSTRUCTORS *************************************
         ***************************************************************************************
         ***************************************************************************************/

        /**
         * @brief Construct a new player object
         * 
         * @param c1 first card
         * @param c2 second card
         * @param r pointer to rounding bet value
         * @param p pointer to current phase board pot
         */
        player(card c1, card c2, float* r, float* p = nullptr);
        
        /**
         * @brief Construct a new player object
         * 
         * @param h hand player
         * @param r pointer to rounding bet value
         * @param p pointer to current phase board pot
         */
        player(hand h, float* r, float* p = nullptr);

        /**
         * @brief Remove default constructor
         * 
         */
        player() = delete;

        /**
         * @brief Destroy the player object
         * 
         */
        ~player();


        /***************************************************************************************
         ***************************************************************************************
         ************************************** FUNCTIONS **************************************
         ***************************************************************************************
         ***************************************************************************************/

        /**
         * @brief Set the blind list index
         * 
         * @param idx index
         */
        inline void set_blind_index(int idx) {blind_index = idx;}

        /**
         * @brief Get the blind combination index list
         * 
         * @return int index of combination in list
         */
        inline int get_blind_index() {return this->blind_index;}

        /**
         * @brief Set the flop list index
         * 
         * @param idx index
         */
        inline void set_flop_index(int idx) {flop_index = idx;}

        /**
         * @brief Get the fold combination index list
         * 
         * @return int index of combination in list
         */
        inline int get_flop_index() {return this->flop_index;}

        /**
         * @brief Get the player last action
         * 
         * @return int last action index
         */
        inline int get_last_action() {return this->last_action;}
        
        /**
         * @brief rank hand resorting to hand dedicated function
         * 
         * @return true if rank is succesful
         * @return false otherwise
         */
        bool rank_hand() {return Hand.rank_hand();}

        /**
         * @brief Get the player hand rank
         * 
         * @return rank structure
         */
        rank get_rank() {return Hand.get_rank();}

        /**
         * @brief link the board to the players hand 
         * 
         * @param board pointer to card vector representing board
         */
        inline void set_board(std::vector<card>* board) {Hand.set_board(board);}
        
        /**
         * @brief clear board
         * 
         */
        inline void clear_board() {Hand.clear_board();}

        /**
         * @brief retrun if player has a valid boad linked
         * 
         */
        inline bool board_valid() {return Hand.board_valid();}

        /**
         * @brief Get the sorted position object
         * 
         * @return std::vector<short> veoctor of ordered position values
         */
        inline std::vector<short> get_sorted_position() {return Hand.get_sorted_position();}

        /**
         * @brief initialize player with a new hand and empty board
         * 
         * @param c1 first card
         * @param c2 second card
         * @param clear 
         * @return true 
         * @return false 
         */
        inline bool new_hand(card c1, card c2, bool clear = true) {Hand.clear_board();
                                                                   return Hand.new_hand(c1, c2, clear); 
                                                                   this->is_active = true;}

        /**
         * @brief initialize player with a new hand and empty board
         * 
         * @param v1 first card position value
         * @param v2 second card position value
         * @param clear flag to clear board link
         * @return true if procedure succeed
         * @return false otherwise
         */
        inline bool new_hand(int v1, int v2, bool clear = true) {Hand.clear_board();
                                                                 return Hand.new_hand(v1, v2, clear);
                                                                 this->is_active = true;}

        /**
         * @brief initialize player from a new provided hand
         * 
         * @param h input hand value
         */
        inline void new_hand(hand h) {Hand = h;}

        /**
         * @brief take check action
         * 
         * @return true if procedure succeed
         * @return false otherwise
         */
        bool check();

        /**
         * @brief take call action
         * 
         * @return true if procedure succeed
         * @return false otherwise
         */
        bool call();

        /**
         * @brief take bet action
         * 
         * @param n amount to bet
         * @param is_preflop flag for preflop action
         * @return true if procedure succeed
         * @return false otherwise
         */
        bool bet(int n, bool is_preflop = false);

        /**
         * @brief take raise action
         * 
         * @param n amount to raise
         * @return true if procedure succeed
         * @return false otherwise
         */
        bool raise(int n);

        /**
         * @brief take fold action
         * 
         * @return true if procedure succeed
         * @return false otherwise
         */
        bool fold();

        /**
         * @brief take allin action
         * 
         * @return true if procedure succeed
         * @return false otherwise
         */
        bool allin();

        /**
         * @brief return player status
         * 
         * @return true if is active
         * @return false if has folded
         */
        bool player_active() {return this->is_active;}

        /**
         * @brief Get the current bet value
         * 
         * @return int bet value
         */
        inline float get_current_bet() {return this->current_bet;}

        /**
         * @brief set the current bet to 0
         * 
         */
        inline void reset_current_bet() {this->current_bet = 0;}

        /**
         * @brief increase stack
         * 
         * @param win amount to increase stack
         */
        inline void pay_win(float win) {this->stack += win;}

        inline float get_stack() {return this->stack;}

        /**
         * @brief print sequence of symbolic cards
         * 
         * @return std::ostream& output stream
         */
        friend std::ostream& operator<<(std::ostream& os, player& p) 
        {   
            os << "Player: " << p.ID << "\n";
            os << p.Hand;
            os << "Current stack: " << p.stack << "\n";
            os << "Current bet: " << p.get_current_bet() << "\n";
            return os;
        }
    };        

}; // namespace poker