#include <iostream>
#include <vector>

#include "Calculations.h"
#include "PolynomialConversionToLatex.h"
#include "StringUtils.h"


int main() {


//    std::string pol1 = "x^5+x^2+x+1";
//    std::string pol2 = "x^2+1";
//    std::string pol1 = "2x^3+x^2+1";
//    std::string pol2 = "2x^2+2x+2";
//    std::string pol1 = "2x^4+x^3+x^2+2x+2";
//    std::string pol2 = "2x+2";
//    std::string pol1 = "3x^5+2x^4+3x^2+x+3";
//    std::string pol2 = "x^2+4x+3";


    std::string pol1;
    std::string pol2;
    std::string mod;
    std::cout << "Put in the first polynomial: ";
    std::cin >> pol1;
    std::cout << std::endl << "Put in the second polynomial: ";
    std::cin >> pol2;

    std::cout << std::endl << "Put in the value of the modulus: ";
    std::cin >> mod;
    int mod_int = std::stoi(mod);

//    int mod_int = 2;
    std::vector<int> pol1_int = StringUtils::polynomial_to_int_array(pol1);
    std::vector<int> pol2_int = StringUtils::polynomial_to_int_array(pol2);

    auto [q , r] = Calculations::polynomial_division_mod(pol1_int, pol2_int, mod_int);

    std::string output = PolynomialConversionToLatex::polynomial_division_to_latex(pol1_int, pol2_int, q, r, mod_int);

    std::cout << output << std::endl;


    return 0;
}
