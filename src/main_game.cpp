/**
 * @file main_game.cpp
 * @author Mattia Scarpa 
 * @brief 
 * @version 0.1
 * @date 2023-03-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <chrono>
#include <memory>
#include "../include/utilities.hpp"
#include "../include/card.hpp"
#include "../include/hand.hpp"
#include "../include/deck.hpp"
#include "../include/combinations.hpp"

int main(int argc, char const *argv[])
{
    poker::card c_test = poker::card(5, 3);
    std::cout << c_test << "\n";

    poker::deck d_test = poker::deck();

    //std::cout << d_test << "\n";

    poker::card c = d_test.draw();

    std::cout << "draw test: " << c << "\n";
    //std::cout << "remaining: " << d_test << "\n";    

    poker::hand h_test{poker::hand(d_test.draw(), d_test.draw())};
    poker::hand h_test2{poker::hand(d_test.draw(), d_test.draw())};

    std::vector<poker::card> board_test{d_test.draw(), d_test.draw(), d_test.draw(), d_test.draw(), d_test.draw()};
    h_test.set_board(&board_test);
    h_test.rank_hand();
    std::cout << h_test;
    int count = 5;

    combinations comb_test;

    long init = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    //#pragma omp parallel for
    for (size_t i = 0; i < count; i++)
    {
        d_test.restore();
        board_test = {d_test.draw(), d_test.draw(), d_test.draw()};
        h_test.clear_board();
        h_test.new_hand(d_test.draw(), d_test.draw());
        h_test.set_board(&board_test);
        h_test.rank_hand();
        h_test2.clear_board();
        h_test2.new_hand(d_test.draw(), d_test.draw());
        h_test2.set_board(&board_test);
        h_test2.rank_hand();
        //std::cout << h_test;    
        std::vector<int> wins = {comb_test.find_combinations(h_test.get_sorted_position()), comb_test.find_combinations(h_test2.get_sorted_position())};
        comb_test.update_winners(wins, {0}, false);
        comb_test.find_combinations(h_test.get_sorted_position());
        comb_test.find_combinations(h_test2.get_sorted_position());
    }
    long end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    comb_test.save_combinations();
    std::cout << "Total time: " << ((end-init)/1000.f/count) << "[\u03BCs]" << "\n";

    return 0;
}
