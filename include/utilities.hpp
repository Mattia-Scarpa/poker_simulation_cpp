/**
 * @file utilities.hpp
 * @author Mattia Scarpa
 * @brief 
 * @version 0.4
 * @date 2023-04-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <vector>

namespace utility
{    
    /**
         * @brief remove a vector element at a given index
         * 
         * @tparam T custom vector data type
         * @param v input vector
         * @param n vector index
         * @return T return vector element removed 
         */
        template <typename T> T remove_at(std::vector<T>&v, typename std::vector<T>::size_type n) {
            T ans = v.at(n);
            v.erase(v.begin() + n);
            return ans;
        }

} // namespace utility
