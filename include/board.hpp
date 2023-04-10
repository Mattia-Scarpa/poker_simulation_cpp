# pragma once


namespace poker
{
    class board
    {
    private:

        const int NUM_PLAYER;

        int TURN = 0;

        deck DECK;

        std::vector<card> BOARD;

        std::vector<hand> PLAYERS_HAND;

    public:

        /**
         * @brief Construct a new board object
         * 
         * @param num_player 
         */
        board(int num_player);
        
        board() = delete;

        /**
         * @brief Destroy the board object
         * 
         */
        ~board();

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

        
    };
    
} // namespace poker
