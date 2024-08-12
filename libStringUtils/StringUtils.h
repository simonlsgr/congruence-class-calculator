//
// Created by simon on 11.08.24.
//

#ifndef CONGRUENCE_CLASS_CALCULATOR_STRINGUTILS_H
#define CONGRUENCE_CLASS_CALCULATOR_STRINGUTILS_H

#include <algorithm>
#include <string>
#include "PolynomialOperations.h"

namespace StringUtils {
    void sort_polynomial (std::vector<std::string> & polynomial);
    std::vector<std::string> string_parser (std::string & str, std::string & delimiter) {
        unsigned long pos;
        std::string out;
        std::vector<std::string> vec_out;
        while ((pos = str.find(delimiter)) != std::string::npos) {
            out = str.substr(0, pos);
            vec_out.push_back(out);
            str.erase(0, pos + delimiter.length());
        }
        vec_out.push_back(str);
        return vec_out;
    }


    int get_monomial_power (std::string & monomial) {
        unsigned long pos_power = monomial.find('^'), pos_x = monomial.find('x');
        if (pos_power == std::string::npos) {
            if (pos_x != std::string::npos) {
                return 1;
            } else {
                return 0;
            }
        }
        return std::stoi(monomial.substr(pos_power+1, monomial.length()));
    }

    int get_monomial_coefficient (std::string & monomial) {
        unsigned long pos_x = monomial.find('x');
        if (pos_x == std::string::npos) return std::stoi(monomial);
        if (pos_x == 0) {
            return 1;
        }
        return std::stoi(monomial.substr(0, pos_x));
    }

    void fill_polynomial (std::vector<std::string> & polynomial) {
        std::vector<std::string> empty_polynomial(get_monomial_power(polynomial[0])+1);
        int counter = 0;
        for (int i = 0; i < empty_polynomial.size(); ++i) {
            if (get_monomial_power(polynomial[counter]) == empty_polynomial.size()-i-1) {
                empty_polynomial[i] = polynomial[counter];
                if (counter < polynomial.size()-1) {
                    ++counter;
                }
            } else {
                empty_polynomial[i] = "0x^"+std::to_string(empty_polynomial.size()-i-1);
            }
        }

        polynomial = empty_polynomial;
    }

    std::vector<int> polynomial_to_int_array(std::string & polynomial) {
        std::string plus = "+";
        std::vector<std::string> vector_of_polynomial = string_parser(polynomial, plus);
        sort_polynomial(vector_of_polynomial);
        fill_polynomial(vector_of_polynomial);

        std::vector<int> out;
        out.reserve(vector_of_polynomial.size());
        for (auto vec : vector_of_polynomial)
            out.push_back(get_monomial_coefficient(vec));

        return out;
    }

    void sort_polynomial (std::vector<std::string> & polynomial) {
        std::sort(polynomial.begin(), polynomial.end(),
                  [](std::string a, std::string b)
                  {
                      return get_monomial_power(a) > get_monomial_power(b);
                  }
        );
    }

    std::vector<std::string> polynomial_int_vector_to_string_vector (std::vector<int> & polynomial) {
        std::vector<std::string> polynomial_string;
        int highest_polynomial_index = std::min(PolynomialOperations::find_index_of_highest_polynomial_power(polynomial), 1);
        if (PolynomialOperations::is_zero_polynomial(polynomial) && polynomial.size() > 1) highest_polynomial_index = 1;
        for (int i = highest_polynomial_index; i < polynomial.size(); ++i) {
            unsigned long power = polynomial.size()-1-i;
            if (power == 0) {
                polynomial_string.push_back(std::to_string(polynomial[i]));
            } else if (power == 1) {
                if (polynomial[i] == 1) {
                    polynomial_string.emplace_back("x");
                } else{
                    polynomial_string.push_back(std::to_string(polynomial[i])+"x");
                }
            } else {
                if (polynomial[i] == 1) {
                    polynomial_string.push_back("x^"+std::to_string(power));
                } else{
                    polynomial_string.push_back(std::to_string(polynomial[i])+"x^"+std::to_string(power));
                }
            }
        }
        return polynomial_string;
    }



    std::string join_with_delimiter (std::vector<std::string> & vec, std::string delimiter) {
        std::string result;
        for (int i = 0; i < vec.size()-1; ++i) {
            result += vec[i] + delimiter;
        }
        result += vec[vec.size()-1];
        return result;
    }

    std::string string_n_x (int n, std::string x) {
        std::string result;
        for (auto i = 0; i < n; ++i) {
            result+=x;
        }
        return result;
    }

}

#endif //CONGRUENCE_CLASS_CALCULATOR_STRINGUTILS_H
