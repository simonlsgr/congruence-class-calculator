//
// Created by simon on 12.08.24.
//

#ifndef CONGRUENCE_CLASS_CALCULATOR_POLYNOMIAL_H
#define CONGRUENCE_CLASS_CALCULATOR_POLYNOMIAL_H

#include <vector>
#include <string>
#include <algorithm>
#include <cstring>

#include "Monomial.h"
#include "Calculations.h"

template <int mod>
class Polynomial {
private:
    std::string empty = "";
public:
    Polynomial() : Polynomial(empty) {
    };

    Polynomial(std::string & polynomial) :
    polynomial_as_string(polynomial) {
        if (polynomial_as_string != "") {
            polynomial_string_to_monomial_vector();
            polynomial_string_to_int_vector();
        }
    };
    Polynomial(size_t deg)
    {
        polynomial_as_int_vector.resize(deg);
        std::memset(polynomial_as_int_vector.data(), 0, sizeof(int)*polynomial_as_int_vector.size());
    }

    Polynomial(std::vector<int> & int_vector) :
    polynomial_as_int_vector(int_vector){
    }


    std::string polynomial_as_string;

    std::vector<Monomial<mod>> polynomial_as_monomials = {};
    std::vector<int> polynomial_as_int_vector = {};

    template<int modulus>
    friend Polynomial<modulus> operator/(const Polynomial<modulus> & polynomial1, const Polynomial<modulus> & polynomial2);

    template<int modulus>
    friend Polynomial<modulus> operator%(const Polynomial<modulus> & polynomial1, const Polynomial<modulus> & polynomial2);


    template<class func>
    void sort_polynomial (func && lambda);
    void polynomial_string_to_int_vector();

    void fill_polynomial ();
    std::vector<Monomial<mod>> polynomial_to_monomial_vector_parser(std::string & delimiter);
    void polynomial_string_to_monomial_vector();
    int degree() const;
};

template <int mod>
Polynomial<mod> operator/(const Polynomial<mod> & dividend, const Polynomial<mod> & divisor) {
    Polynomial<mod> quotient(dividend.degree()-divisor.degree()+1);
    std::vector<Polynomial<mod>> remainders;
    std::vector<Polynomial<mod>> subtrahends;
    remainders.push_back(dividend);

    int counter = 0;
    while (divisor.degree() <= dividend.degree() - counter) {

        quotient.polynomial_as_int_vector[dividend.degree()-divisor.degree()-counter] = Calculations::positive_modulo(
                Calculations::multiplicative_inverse(mod, divisor.polynomial_as_int_vector[divisor.degree()])*
                remainders[counter].polynomial_as_int_vector[dividend.degree() - counter],
                mod);

        subtrahends.push_back(Polynomial<mod>(dividend.degree()+1));
        remainders.push_back(Polynomial<mod>(dividend.degree()+1));

        for (int i = dividend.degree() - counter; i >= 0; --i) {
            if (i + counter - quotient.degree() >= 0) {
                subtrahends[counter].polynomial_as_int_vector[i] =
                        Calculations::positive_modulo(
                                quotient.polynomial_as_int_vector[quotient.degree() - counter] *
                                divisor.polynomial_as_int_vector[i + counter - quotient.degree()],
                                mod);
                remainders[counter + 1].polynomial_as_int_vector[i] = Calculations::positive_modulo(remainders[counter].polynomial_as_int_vector[i]
                                                                                                    -
                                                                                                    subtrahends[counter].polynomial_as_int_vector[i],
                                                                                                    mod);
            } else {
                remainders[counter + 1].polynomial_as_int_vector[i] = remainders[counter].polynomial_as_int_vector[i];
            }
        }
        ++counter;


    }
    return quotient;
}

template <int mod>
Polynomial<mod> operator%(const Polynomial<mod> & dividend, const Polynomial<mod> & divisor) {
    Polynomial<mod> quotient(dividend.degree()-divisor.degree()+1);
    std::vector<Polynomial<mod>> remainders;
    std::vector<Polynomial<mod>> subtrahends;
    remainders.push_back(dividend);

    int counter = 0;
    while (divisor.degree() <= dividend.degree() - counter) {

        quotient.polynomial_as_int_vector[dividend.degree()-divisor.degree()-counter] = Calculations::positive_modulo(
                Calculations::multiplicative_inverse(mod, divisor.polynomial_as_int_vector[divisor.degree()])*
                remainders[counter].polynomial_as_int_vector[dividend.degree() - counter],
                mod);

        subtrahends.push_back(Polynomial<mod>(dividend.degree()+1));
        remainders.push_back(Polynomial<mod>(dividend.degree()+1));

        for (int i = dividend.degree() - counter; i >= 0; --i) {
            if (i + counter - quotient.degree() >= 0) {
                subtrahends[counter].polynomial_as_int_vector[i] =
                        Calculations::positive_modulo(
                                quotient.polynomial_as_int_vector[quotient.degree() - counter] *
                                divisor.polynomial_as_int_vector[i + counter - quotient.degree()],
                                mod);
                remainders[counter + 1].polynomial_as_int_vector[i] = Calculations::positive_modulo(remainders[counter].polynomial_as_int_vector[i]
                                                                                                    -
                                                                                                    subtrahends[counter].polynomial_as_int_vector[i],
                                                                                                    mod);
            } else {
                remainders[counter + 1].polynomial_as_int_vector[i] = remainders[counter].polynomial_as_int_vector[i];
            }
        }
        ++counter;


    }
    return remainders[remainders.size()-1];
}


template <int mod>
int Polynomial<mod>::degree() const {
    auto rit = std::find_if(std::rbegin(polynomial_as_int_vector), std::rend(polynomial_as_int_vector), [](int i){return i != 0;});
    if (rit != std::rend(polynomial_as_int_vector)) {
        return std::distance(rit, polynomial_as_int_vector.rend()) - 1;
    } else {
        return 0;
    }
}

template <int mod>
template <class func>
void Polynomial<mod>::sort_polynomial (func && lambda) {
    std::sort(polynomial_as_monomials.begin(), polynomial_as_monomials.end(), lambda);
}



template<int mod>
void Polynomial<mod>::polynomial_string_to_int_vector() {
    polynomial_as_int_vector.clear();
    polynomial_as_int_vector.reserve(polynomial_as_monomials.size());
    for (auto monomial : polynomial_as_monomials) {
        polynomial_as_int_vector.push_back(monomial.get_monomial_coefficient());
    }
}

template<int mod>
void Polynomial<mod>::fill_polynomial () {
    std::vector<Monomial<mod>> empty_polynomial(polynomial_as_monomials[0].get_monomial_power()+1);
    int counter = 0;
    for (int i = 0; i < empty_polynomial.size(); ++i) {
        if (polynomial_as_monomials[counter].get_monomial_power() == empty_polynomial.size()-i-1) {
            empty_polynomial[i] = polynomial_as_monomials[counter];
            if (counter < polynomial_as_monomials.size()-1) {
                ++counter;
            }
        } else {
            std::string monomial_string = "0x^"+std::to_string(empty_polynomial.size()-i-1);
            Monomial<mod> monomial(monomial_string);
            empty_polynomial[i] = monomial;
        }
    }

    polynomial_as_monomials = empty_polynomial;
}

template<int mod>
void Polynomial<mod>::polynomial_string_to_monomial_vector() {
    std::string plus = "+";
    polynomial_as_monomials = polynomial_to_monomial_vector_parser(plus);
    sort_polynomial(
            [](Monomial<mod> a, Monomial<mod> b)
                    {
                        return a > b;
                    });
    fill_polynomial();
    sort_polynomial(
            [](Monomial<mod> a, Monomial<mod> b)
            {
                return a < b;
            });

}

template <int mod>
std::vector<Monomial<mod>> Polynomial<mod>::polynomial_to_monomial_vector_parser(std::string & delimiter) {
    unsigned long pos;
    std::string polynomial_as_string_copy = polynomial_as_string;
    std::string out;
    std::vector<Monomial<mod>> vec_out;
    while ((pos = polynomial_as_string_copy.find(delimiter)) != std::string::npos) {
        out = polynomial_as_string_copy.substr(0, pos);
        vec_out.push_back(out);
        polynomial_as_string_copy.erase(0, pos + delimiter.length());
    }
    vec_out.push_back(polynomial_as_string_copy);
    return vec_out;
}

#endif //CONGRUENCE_CLASS_CALCULATOR_POLYNOMIAL_H
