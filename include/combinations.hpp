
#pragma once

constexpr int COMBS_2CARD{1326};                // Total of 2 cards combination
constexpr int COMBS_5CARD{2598960};             // Total of 5 cards combination

class combinations
{
private:
    /**
     * @brief containers for 2 cards combinations
     * 
     */
    std::vector<std::vector<short>> combs2;
    /**
     * @brief containers for 5 cards combinations
     * 
     */
    std::vector<std::vector<short>> combs5;


    // Initialize result container for Win [Tie [Lose]]
    std::vector<std::vector<float>> oddsResultBlind;                     // 2 cards result (blind)
    std::vector<std::vector<float>> oddsResultFlop;                      // 5 cards result (flop)

    int PLAYER_COUNT;

    /**
     * @brief phase of game managed
     * 
     */
    const std::vector<std::string> GAME_PHASE = {"_blind_", "_flop_"};                // phase of the game saved

    static bool compare_positions(const std::vector<short>& comb, const std::vector<int>& target) {
    for (size_t i(0); i<target.size(); i++) {
        if (comb[i] < target[i]) {
            return true;
        }
    }
    return false;
    }

    /**
     * @brief perform binary search in an ordered 2D vector
     * 
     * @param array 2D vector to search
     * @param value vector to find
     * @return int index in array
     */
    int binary_search(std::vector<std::vector<short>>& array, const std::vector<short>& value);
    
public:

    /***************************************************************************************
     ***************************************************************************************
     ************************************ CONSTRUCTORS *************************************
     ***************************************************************************************
     ***************************************************************************************/

    /**
     * @brief Construct a new combinations object
     * 
     * @param player count, how many player are in the table
     */
    combinations(int player = 2);
    
    /**
     * @brief Destroy the combinations object
     * 
     */
    ~combinations();

    /***************************************************************************************
     ***************************************************************************************
     ************************************** FUNCTIONS **************************************
     ***************************************************************************************
     ***************************************************************************************/

    /**
     * @brief Search target vector in the combination containers
     * 
     * @param target vector to find
     * @return int index in corresponding vector (if found, -1 otherwise)
     */
    int find_combinations(const std::vector<short>& target);

    /**
     * @brief save combination container to txt file
     * 
     * @param PLAYER_COUNT player count
     * @return true if write has succes
     * @return false otherwise
     */
    bool save_combinations();

    /**
     * @brief insert win/lose list according to index found
     * 
     * @param hands_index index in combinations lists for each hands
     * @param winners_index index from hands_index that are winning hands
     * @param blind parameters that is true if blind list
     */
    inline void update_winners(std::vector<int> hands_index, std::vector<int> winners_index, int blind = true) 
    {
        for (int hands_idx : hands_index) {
            if (blind)
                oddsResultBlind[hands_idx][1]++;          // Increase the hand count by one
            else
                oddsResultFlop[hands_idx][1]++;
        }
        
        // Incrementing the winner combination in the games
        // If more than one player won on the same simulation the weight will be lower according to the winners count
        for (int winners_idx : winners_index) {
            if (blind)
                oddsResultBlind[hands_index[winners_idx]][0]++;
            else
                oddsResultFlop[hands_index[winners_idx]][0]++;

            //std::cout << "Writing at: " << hands_index[winners_idx] << "\n";     
            }   
    }

};
