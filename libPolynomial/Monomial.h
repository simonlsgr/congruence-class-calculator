//
// Created by simon on 12.08.24.
//

#ifndef CONGRUENCE_CLASS_CALCULATOR_MONOMIAL_H
#define CONGRUENCE_CLASS_CALCULATOR_MONOMIAL_H

#include <string>

template <int mod>
class Monomial {
public:

    Monomial() {};

    Monomial(std::string & monomial) : monomial_as_string(monomial) {

    };

    bool operator<(const Monomial &rhs) const {
        if (get_monomial_power()  < rhs.get_monomial_power()) {
            return true;
        } else {
            return false;
        }
    }

    bool operator>(const Monomial &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const Monomial &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Monomial &rhs) const {
        return !(*this < rhs);
    }

    int modulus = mod;
    std::string monomial_as_string;
    int get_monomial_power() const;
    int get_monomial_coefficient() const;
};


template <int mod>
int Monomial<mod>::get_monomial_power() const {
    unsigned long pos_power = monomial_as_string.find('^'), pos_x = monomial_as_string.find('x');
    if (pos_power == std::string::npos) {
        if (pos_x != std::string::npos) {
            return 1;
        } else {
            return 0;
        }
    }
    return std::stoi(monomial_as_string.substr(pos_power+1, monomial_as_string.length()));
}

template <int mod>
int Monomial<mod>::get_monomial_coefficient () const {
    unsigned long pos_x = monomial_as_string.find('x');
    if (pos_x == std::string::npos) return std::stoi(monomial_as_string);
    if (pos_x == 0) {
        return 1;
    }
    return std::stoi(monomial_as_string.substr(0, pos_x));
}

#endif //CONGRUENCE_CLASS_CALCULATOR_MONOMIAL_H
