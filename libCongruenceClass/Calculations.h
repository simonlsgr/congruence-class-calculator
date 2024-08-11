//
// Created by simon on 11.08.24.
//

#ifndef CONGRUENCE_CLASS_CALCULATOR_CALCULATIONS_H
#define CONGRUENCE_CLASS_CALCULATOR_CALCULATIONS_H


#include <tuple>

#include "PolynomialOperations.h"

namespace Calculations {
    int positive_modulo(int i, int n) {
        return (i % n + n) % n;
    }
    std::tuple<int, int, int> extended_euclidean_algorithm (int a, int b) {
        int remainder_i_minus_1 = std::max(a, b),
                remainder_i = std::min(a, b);
        int quotient;
        int s_i_minus_1 = 1,    t_i_minus_1 = 0,
                s_i = 0,            t_i = 1;


        while (remainder_i != 0) {
            int temp_remainder = remainder_i;
            int temp_s_i = s_i,
                    temp_t_i = t_i;
            quotient = remainder_i_minus_1 / remainder_i;

            s_i = (-quotient) * s_i + s_i_minus_1;
            s_i_minus_1 = temp_s_i;
            t_i = (-quotient) * t_i + t_i_minus_1;
            t_i_minus_1 = temp_t_i;

            remainder_i = remainder_i_minus_1 - (remainder_i * quotient);
            remainder_i_minus_1 = temp_remainder;

        }
        return std::make_tuple(remainder_i_minus_1, s_i_minus_1, t_i_minus_1);
    }

    int multiplicative_inverse (int mod, int number){
        auto [gcd, s, t] = extended_euclidean_algorithm(mod, number%mod);
        if (gcd != 1) return 0;
        return positive_modulo(t, mod);
    }

    std::tuple<std::vector<int>, std::vector<std::vector<int>>> polynomial_division_mod (std::vector<int> & f, std::vector<int> & g, int mod) {
        std::vector<int> q;
        std::vector<std::vector<int>> r;
        std::vector<std::vector<int>> subtrahends;
        r.push_back(f);
        std::vector<int> r0;
        r.push_back(r0);
        subtrahends.push_back(r0);
        PolynomialOperations::fill_0_in_vector(q , f.size() - g.size() + 1);
        r[0].insert(r[0].begin(), 0);
        PolynomialOperations::fill_0_in_vector(r[1], r[0].size()-1);
        PolynomialOperations::fill_0_in_vector(subtrahends[0], r[0].size()-1);
        int counter = 0;
        while (g.size() <= r[counter].size() - PolynomialOperations::find_index_of_highest_polynomial_power(r[counter])) {

            q[counter] = Calculations::positive_modulo(
                    Calculations::multiplicative_inverse(mod, g[0])*
                    r[counter][1]
                    ,mod);
            for (int i = 0; i < r[counter].size(); ++i) {
                if (i < g.size()) {
                    subtrahends[counter][i] =
                            Calculations::positive_modulo(q[counter] * g[i], mod);
                    r[counter+1][i] = Calculations::positive_modulo(r[counter][i+1] - subtrahends[counter][i], mod);
                } else {
                    r[counter+1][i] = Calculations::positive_modulo(r[counter][i+1], mod);
                }
            }
            ++counter;
            r.push_back(r0);
            subtrahends.push_back(r0);
            PolynomialOperations::fill_0_in_vector(r[counter+1], r[counter].size()-1);
            PolynomialOperations::fill_0_in_vector(subtrahends[counter], subtrahends[counter-1].size()-1);

        }
        r.pop_back();
        subtrahends.pop_back();
        PolynomialOperations::merge_subtrahends_into_r(r, subtrahends);
        return std::make_tuple(q, r);

    }
}


#endif //CONGRUENCE_CLASS_CALCULATOR_CALCULATIONS_H
