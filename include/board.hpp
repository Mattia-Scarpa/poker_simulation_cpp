# pragma once


namespace poker
{

    #define BLIND 0
    #define FLOP 1
    #define TURN 2
    #define RIVER 3

    #define BB 100
    #define SB 50

    class board
    {
    private:

        /**
         * @brief number of initial player counter
         * 
         */
        const int NUM_PLAYER;
        bool first_init{true};

        /**
         * @brief turn counter:
         * 0 -> blind, 1 -> flop, 2 -> turn, 3 -> river.
         * 
         */
        int PHASE{0};

        /**
         * @brief game board pot
         * 
         */
        int TOTAL_POT{0};
        int CURRENT_POT{0};

        /**
         * @brief offset to determine the first player acting
         * 
         */
        int position_offset{0};
        int rounding_bet{0};

        /**
         * @brief board deck cards 
         * 
         */
        deck DECK;

        /**
         * @brief vector of card represending current board
         * 
         */
        std::vector<card> BOARD;

        /**
         * @brief vector of active player
         * 
         */
        std::vector<player> PLAYERS_HAND;

        /**
         * @brief index of winner in player vector
         * 
         */
        std::vector<int> winner_player_index;

        /**
         * @brief combination object
         * 
         */
        combinations COMBS;
        
        bool flag_save;

        /**
         * @brief initialize board drwaing hand according to new player
         * 
         */
        void init();

        /**
         * @brief draw new cards for board
         * 
         */
        bool draw_board();

        /**
         * @brief player action manager
         * 
         * @param player_index active player index
         * @return int action taken
         */
        int select_action(int player_index);

    public:

        /**
         * @brief Construct a new board object
         * 
         * @param num_player 
         */
        board(int num_player, bool save_result = true);
        
        board() = delete;

        /**
         * @brief Destroy the board object
         * 
         */
        ~board();

        /**
         * @brief play the turn
         * 
         * @return true 
         * @return false 
         */
        bool play_turn();

        /**
         * @brief set the board to begin a new game with new players
         * 
         * @return true 
         * @return false 
         */
        inline void set_new_game() {PLAYERS_HAND.clear(); first_init = true;}

        /**
         * @brief Get the winner list
         * 
         * @return std::vector<player*> pointer to winners players
         */
        std::vector<player*> get_winner();

        /**
         * @brief print board representation
         * 
         * @return std::ostream& output stream
         */
        friend std::ostream& operator<<(std::ostream& os, const board& b) 
        {   

            os << "PHASE:" << b.PHASE << "\n";
            os << "TOTAL POT: " << b.TOTAL_POT << "\n";
            os << "CURRENT POT: " << b.CURRENT_POT << "\n";
            os << "ROUNDING BET: " << b.rounding_bet << "\n";

            for (player player:b.PLAYERS_HAND)
            {
                if (player.player_active())
                {
                    std::cout << player;
                    std:: cout << "\n---------------------\n";
                }
            }
            return os;
        }
        
    };
    
} // namespace poker
