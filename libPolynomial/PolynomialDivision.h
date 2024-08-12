//
// Created by simon on 12.08.24.
//

#ifndef CONGRUENCE_CLASS_CALCULATOR_POLYNOMIALDIVISION_H
#define CONGRUENCE_CLASS_CALCULATOR_POLYNOMIALDIVISION_H


#include <vector>
#include <algorithm>
#include "Calculations.h"

template <int mod>
class PolynomialDivsion {
public:
    PolynomialDivsion() = delete;
    PolynomialDivsion(std::vector<int> const & dividend, std::vector<int> const & divisor) : dividend(dividend), divisor(divisor) {
        quotient.resize(degree(dividend)-degree(divisor)+1);
        remainders.push_back(dividend);

        divide();

    };

    std::vector<int> dividend;
    std::vector<int> divisor;


    std::vector<int> quotient;
    std::vector<std::vector<int>> remainders;
    std::vector<std::vector<int>> subtrahends;

    int degree (std::vector<int> const & polyonmial_int_vector);
    void divide();
    int get_current_quotient (int counter);
    void calculate_all_subtrahends_and_remainders (int counter);
    void calculate_subtrahend_and_remainder (int counter, int i);
    void calculate_remainder (int counter, int i);
};


template<int mod>
int PolynomialDivsion<mod>::degree(const std::vector<int> &polyonmial_int_vector) {
    auto rit = std::find_if(std::rbegin(polyonmial_int_vector), std::rend(polyonmial_int_vector), [](int i){return i != 0;});
    if (rit != std::rend(polyonmial_int_vector)) {
        return std::distance(rit, polyonmial_int_vector.rend()) - 1;
    } else {
        return 0;
    }
}

template<int mod>
void PolynomialDivsion<mod>::calculate_subtrahend_and_remainder(int counter, int i) {
    subtrahends[counter][i] =
            Calculations::positive_modulo(
                    quotient[degree(quotient) - counter] *
                    divisor[i + counter - degree(quotient)],
                    mod);
    remainders[counter + 1][i] = Calculations::positive_modulo(
                                                            remainders[counter][i] -
                                                            subtrahends[counter][i],
                                                            mod);
}

template<int mod>
void PolynomialDivsion<mod>::calculate_remainder(int counter, int i) {
    remainders[counter + 1][i] = remainders[counter][i];
}

template<int mod>
void PolynomialDivsion<mod>::calculate_all_subtrahends_and_remainders(int counter) {
    for (int i = degree(dividend) - counter; i >= 0; --i) {
        if (i + counter - degree(quotient) >= 0) {
            calculate_subtrahend_and_remainder(counter, i);
        } else {
            calculate_remainder(counter, i);
        }
    }
}

template<int mod>
int PolynomialDivsion<mod>::get_current_quotient (int counter) {
    return Calculations::positive_modulo(
            Calculations::multiplicative_inverse(mod, divisor[degree(divisor)])*
            remainders[counter][degree(dividend) - counter],
            mod
    );
}

template<int mod>
void PolynomialDivsion<mod>::divide(){
    int counter = 0;
    while (degree(divisor) <= degree(dividend) - counter) {
        quotient[degree(dividend)-degree(divisor)-counter] = get_current_quotient(counter);
        subtrahends.push_back(std::vector<int>(degree(dividend)+1));
        remainders.push_back(std::vector<int>(degree(dividend)+1));
        calculate_all_subtrahends_and_remainders(counter);
        ++counter;
    }
}

#endif //CONGRUENCE_CLASS_CALCULATOR_POLYNOMIALDIVISION_H
