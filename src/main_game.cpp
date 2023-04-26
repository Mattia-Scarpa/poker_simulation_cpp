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
#include "../include/player.hpp"
#include "../include/board.hpp"

int main(int argc, char const *argv[])
{
    int count = 1;

    //combinations comb_test(9);

    std::cout << "\nLOOP\n";

    long init = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
 
    for (size_t i = 0; i < count; i++)
    {
        poker::board Board(3);
        Board.set_new_game();
        Board.play_turn();
    }
    long end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    //comb_test.save_combinations();
    std::cout << "Total time: " << ((end-init)/1000.f/count) << "[\u03BCs]" << "\n";

    return 0;
}


    // poker::card c_test = poker::card(5, 3);
    // std::cout << c_test << "\n";

    // poker::deck deck_test = poker::deck();

    //std::cout << d_test << "\n";

    //poker::card c = d_test.draw();

    //std::cout << "draw test: " << c << "\n";
    //std::cout << "remaining: " << d_test << "\n";    

    //poker::hand h_test{poker::hand(d_test.draw(), d_test.draw())};
    //poker::hand h_test2{poker::hand(d_test.draw(), d_test.draw())};

    //std::vector<poker::card> board_test{deck_test.draw(), deck_test.draw(), deck_test.draw(), deck_test.draw(), deck_test.draw()};
    //h_test.set_board(&board_test);
    //h_test.rank_hand();
    //std::cout << h_test;
    

    //poker::player p(deck_test.draw_hand());
    //p.set_board(&board_test);

    //std::cout << p;

/* for loop test

        deck_test.restore();
        board_test = {deck_test.draw(), deck_test.draw(), deck_test.draw()};
        

        
        poker::player p1(deck_test.draw_hand());
        poker::player p2(deck_test.draw_hand());
        poker::player p3(deck_test.draw_hand());
        poker::player p4(deck_test.draw_hand());
        poker::player p5(deck_test.draw_hand());
        poker::player p6(deck_test.draw_hand());
        poker::player p7(deck_test.draw_hand());
        poker::player p8(deck_test.draw_hand());
        poker::player p9(deck_test.draw_hand());
        p1.set_board(&board_test);
        p2.set_board(&board_test);
        p3.set_board(&board_test);
        p4.set_board(&board_test);
        p5.set_board(&board_test);
        p6.set_board(&board_test);
        p7.set_board(&board_test);
        p8.set_board(&board_test);
        p9.set_board(&board_test);
        //std::cout << p;
        p1.rank_hand();
        p2.rank_hand();
        p3.rank_hand();
        p4.rank_hand();
        p5.rank_hand();
        p6.rank_hand();
        p7.rank_hand();
        p8.rank_hand();
        p9.rank_hand();
        
        std::vector<int> p_idx = {comb_test.find_combinations(p1.get_sorted_position()),
                                  comb_test.find_combinations(p2.get_sorted_position()),
                                  comb_test.find_combinations(p3.get_sorted_position()),
                                  comb_test.find_combinations(p4.get_sorted_position()),
                                  comb_test.find_combinations(p5.get_sorted_position()),
                                  comb_test.find_combinations(p6.get_sorted_position()),
                                  comb_test.find_combinations(p7.get_sorted_position()),
                                  comb_test.find_combinations(p8.get_sorted_position()),
                                  comb_test.find_combinations(p9.get_sorted_position())};
        std::vector<int> w_idx = {0, 3, 4};
        comb_test.update_winners(p_idx, w_idx, false);*/


        //std::cout << p;
        /*d_test.restore();
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
        comb_test.find_combinations(h_test2.get_sorted_position());*/