//
// Created by simon on 11.08.24.
//

#ifndef CONGRUENCE_CLASS_CALCULATOR_POLYNOMIALCONVERSIONTOLATEX_H
#define CONGRUENCE_CLASS_CALCULATOR_POLYNOMIALCONVERSIONTOLATEX_H


#include <string>
#include <vector>

#include "StringUtils.h"

namespace PolynomialConversionToLatex {
    std::string polynomial_to_latex(std::vector<std::string> & polynomial, bool parenthesis) {
        std::string result;
        int index_shift = 0;
        if (parenthesis) {
            index_shift = 1;
            result += "\t&{}\t("+polynomial[0]+"\\hphantom{)}" + "&{}+";
        }
        for (int i = index_shift; i < polynomial.size()-1; ++i) {
            result += "\t&{}\t\\hphantom{(}"+polynomial[i] + "\\hphantom{)}&{}+";
        }
        if (parenthesis) {
            result += "\t&{}\t\\hphantom{(}"+polynomial[polynomial.size()-1]+")";
        } else {
            result += "\t&{}\t\\hphantom{(}"+polynomial[polynomial.size()-1]+"\\hphantom{)}";
        }

        return result;

    }


    std::string polynomial_division_to_latex (std::vector<int> & f, std::vector<int> & g, std::vector<int> & q, std::vector<std::vector<int>> & r, int mod) {
        std::string result = "$\\mathbb{Z}_{"+ std::to_string(mod) + "}:$\n\\vspace{12pt}\n\\\\$\\begin{array}{";

        for (int i = 0; i < f.size(); ++i) {
            result += "c@{} r@{} ";
        }
        result += "r}";
        std::vector<std::string> f_as_string = StringUtils::polynomial_int_vector_to_string_vector(f);
        std::vector<std::string> g_as_string = StringUtils::polynomial_int_vector_to_string_vector(g);
        std::vector<std::string> q_as_string = StringUtils::polynomial_int_vector_to_string_vector(q);
        std::vector<std::vector<std::string>> r_as_string;
        for (int i = 1; i < r.size(); i+=2) {
            if (PolynomialOperations::is_zero_polynomial(r[i])) r[i].push_back(0);
        }
        for (auto r_i : r) r_as_string.push_back(StringUtils::polynomial_int_vector_to_string_vector(r_i));
        result += polynomial_to_latex(f_as_string, false) + "& =";
        result += "(" + StringUtils::join_with_delimiter(g_as_string, "+") + ")\\cdot";
        result += "(" + StringUtils::join_with_delimiter(q_as_string, "+") + ")+";
        result += "(" + StringUtils::join_with_delimiter(r_as_string[r_as_string.size()-1], "+") + ")\\\\ \n";

        for (int i = 1; i < r_as_string.size(); ++i) {
            if (i % 2 != 0) {
                result += StringUtils::string_n_x(i/2*2,"&")+"-"+polynomial_to_latex(r_as_string[i], true) + "\\\\ \n";
                result += "\\cmidrule{"+ std::to_string(2+i/2*2) + "-" + std::to_string(f.size()*2) +"}";
            } else {
                result += StringUtils::string_n_x(i/2*2,"&")+polynomial_to_latex(r_as_string[i], false) + "\\\\ \n";
            }
        }

        result += "\\end{array}$";



        return result;
    }

}


#endif //CONGRUENCE_CLASS_CALCULATOR_POLYNOMIALCONVERSIONTOLATEX_H
