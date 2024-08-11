//
// Created by simon on 11.08.24.
//

#ifndef CONGRUENCE_CLASS_CALCULATOR_POLYNOMIALOPERATIONS_H
#define CONGRUENCE_CLASS_CALCULATOR_POLYNOMIALOPERATIONS_H

#include <vector>
#include <algorithm>

namespace PolynomialOperations {
    void fill_0_in_vector (std::vector<int> & vec, int n) {
        for (int i = 0; i < n; ++i) {
            vec.push_back(0);
        }
    }

    int find_index_of_highest_polynomial_power (std::vector<int> & polynomial) {
        for (int i = 0; i < polynomial.size(); ++i) {
            if (polynomial[i] != 0) {
                return i;
            }
        }
        return 0;
    }

    void merge_subtrahends_into_r (std::vector<std::vector<int>> & r, std::vector<std::vector<int>> & subtrahends) {
        auto pointer = r.begin();
        for (int i = 0; i < subtrahends.size(); ++i) {
            r.insert(r.begin() + 1 + i*2, subtrahends[i]);
        }
    }


    bool is_zero_polynomial (std::vector<int> & polynomial) {
        return std::find_if(std::begin(polynomial), std::end(polynomial), [](int i){return i != 0;}) == std::end(polynomial);
    }
}
#endif //CONGRUENCE_CLASS_CALCULATOR_POLYNOMIALOPERATIONS_H
