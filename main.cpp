#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>
#include <numeric>

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
    unsigned long pos_power = monomial.find("^"), pos_x = monomial.find("x");
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
    unsigned long pos_x = monomial.find("x");
    if (pos_x == std::string::npos) return std::stoi(monomial);
    if (pos_x == 0) {
        return 1;
    }
    return std::stoi(monomial.substr(0, pos_x));
}

void sort_polynomial (std::vector<std::string> & polynomial) {
    std::sort(polynomial.begin(), polynomial.end(),
                [](std::string a, std::string b)
                    {
                        return get_monomial_power(a) > get_monomial_power(b);
                    }
    );
}

void fill_polynomial (std::vector<std::string> & polynomial) {
    std::vector<std::string> empty_polynomial(get_monomial_power(polynomial[0])+1);
    int counter = 0, length_polynomial = polynomial.size();
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
    for (auto vec : vector_of_polynomial)
        out.push_back(get_monomial_coefficient(vec));

    return out;
}

void fill_0_in_vector (std::vector<int> & vec, int n) {
    for (int i = 0; i < n; ++i) {
        vec.push_back(0);
    }

}

int find_index_of_highest_polynomial_power (std::vector<int> & polynomial) {
    for (int i = 0; i < polynomial.size(); ++i) {
        if (polynomial[i] != 0) {
            int ret = i;
            return i;
        }
    }
    return 0;
}

void merge_subtrahends_into_r (std::vector<std::vector<int>> & r, std::vector<std::vector<int>> & subtrahends) {
    std::vector<std::vector<int>>::iterator pointer = r.begin();
    for (int i = 0; i < subtrahends.size(); ++i) {
        r.insert(r.begin() + 1 + i*2, subtrahends[i]);
    }
}

std::tuple<std::vector<int>, std::vector<std::vector<int>>> polynomial_division_mod (std::vector<int> & f, std::vector<int> & g, int mod) {
    std::vector<int> q;
    std::vector<std::vector<int>> r;
    std::vector<std::vector<int>> subtrahends;
    r.push_back(f);
    std::vector<int> r0;
    r.push_back(r0);
    subtrahends.push_back(r0);
    fill_0_in_vector(q , f.size() - g.size() + 1);
    r[0].insert(r[0].begin(), 0);
    fill_0_in_vector(r[1], r[0].size()-1);
    fill_0_in_vector(subtrahends[0], r[0].size()-1);
    int counter = 0;
    while (g.size() <= r[counter].size() - find_index_of_highest_polynomial_power(r[counter])) {

        q[counter] = positive_modulo(
                multiplicative_inverse(mod, g[0])*
                r[counter][1]
                ,mod);
//        for (int i = 0; i < g.size()+q.size(); ++i) {
        for (int i = 0; i < r[counter].size(); ++i) {
            if (i < g.size()) {
                subtrahends[counter][i] =
                        positive_modulo(q[counter] * g[i], mod);
                r[counter+1][i] = positive_modulo(r[counter][i+1] - subtrahends[counter][i], mod);
            } else {
                r[counter+1][i] = positive_modulo(r[counter][i+1], mod);
            }
        }
        ++counter;
        r.push_back(r0);
        subtrahends.push_back(r0);
        fill_0_in_vector(r[counter+1], r[counter].size()-1);
        fill_0_in_vector(subtrahends[counter], subtrahends[counter-1].size()-1);

    }
    r.pop_back();
    subtrahends.pop_back();
    merge_subtrahends_into_r(r, subtrahends);
    return std::make_tuple(q, r);;

}

bool is_zero_polynomial (std::vector<int> & polynomial) {
    for (auto coefficient : polynomial) {
        if (coefficient != 0) return false;
    }
    return true;
}

std::vector<std::string> polynomial_int_vector_to_string_vector (std::vector<int> & polynomial) {
    std::vector<std::string> polynomial_string;
    int highest_polynomial_index = std::min(find_index_of_highest_polynomial_power(polynomial), 1);
    if (is_zero_polynomial(polynomial) && polynomial.size() > 1) highest_polynomial_index = 1;
    for (int i = highest_polynomial_index; i < polynomial.size(); ++i) {
        int power = polynomial.size()-1-i;
        if (power == 0) {
            polynomial_string.push_back(std::to_string(polynomial[i]));
        } else if (power == 1) {
            if (polynomial[i] == 1) {
                polynomial_string.push_back("x");
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

std::string polynomial_to_latex(std::vector<std::string> & polynomial, bool parenthesis) {
    std::string result = "";
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

std::string join_with_delimiter (std::vector<std::string> & vec, std::string delimiter) {
    std::string result = "";
    for (int i = 0; i < vec.size()-1; ++i) {
        result += vec[i] + delimiter;
    }
    result += vec[vec.size()-1];
    return result;
}

std::string string_n_x (int n, std::string x) {
    std::string result = "";
    for (auto i = 0; i < n; ++i) {
        result+=x;
    }
    return result;
}

std::string polynomial_division_to_latex (std::vector<int> & f, std::vector<int> & g, std::vector<int> & q, std::vector<std::vector<int>> & r, int mod) {
    std::string result = "$\\mathbb{Z}_{"+ std::to_string(mod) + "}:$\n\\vspace{12pt}\n\\\\$\\begin{array}{";

    for (int i = 0; i < f.size(); ++i) {
        result += "c@{} r@{} ";
    }
    result += "r}";
    std::vector<std::string> f_as_string = polynomial_int_vector_to_string_vector(f);
    std::vector<std::string> g_as_string = polynomial_int_vector_to_string_vector(g);
    std::vector<std::string> q_as_string = polynomial_int_vector_to_string_vector(q);
    std::vector<std::vector<std::string>> r_as_string;
    for (int i = 1; i < r.size(); i+=2) {
        if (is_zero_polynomial(r[i])) r[i].push_back(0);
    }
    for (auto r_i : r) r_as_string.push_back(polynomial_int_vector_to_string_vector(r_i));
    result += polynomial_to_latex(f_as_string, false) + "& =";
    result += "(" + join_with_delimiter(g_as_string, "+") + ")\\cdot";
    result += "(" + join_with_delimiter(q_as_string, "+") + ")+";
    result += "(" + join_with_delimiter(r_as_string[r_as_string.size()-1], "+") + ")\\\\ \n";

    for (int i = 1; i < r_as_string.size(); ++i) {
        if (i % 2 != 0) {
            result += string_n_x(i/2*2,"&")+"-"+polynomial_to_latex(r_as_string[i], true) + "\\\\ \n";
            result += "\\cmidrule{"+ std::to_string(2+i/2*2) + "-" + std::to_string(f.size()*2) +"}";
        } else {
            result += string_n_x(i/2*2,"&")+polynomial_to_latex(r_as_string[i], false) + "\\\\ \n";
        }
    }

    result += "\\end{array}$";



    return result;
}

int main() {


//    std::string pol1 = "x^5+x^2+x+1";
//    std::string pol2 = "x^2+1";
//    std::string pol1 = "2x^3+x^2+1";
//    std::string pol2 = "2x^2+2x+2";
//    std::string pol1 = "2x^4+x^3+x^2+2x+2";
//    std::string pol2 = "2x+2";
//    std::string pol1 = "3x^5+2x^4+3x^2+x+3";
//    std::string pol2 = "x^2+4x+3";


    std::string pol1 = "";
    std::string pol2 = "";
    std::string mod = "";
    std::cout << "Put in the first polynomial: ";
    std::cin >> pol1;
    std::cout << std::endl << "Put in the second polynomial: ";
    std::cin >> pol2;

    std::cout << std::endl << "Put in the value of the modulus: ";
    std::cin >> mod;
    int mod_int = std::stoi(mod);

    std::vector<int> pol1_int = polynomial_to_int_array(pol1);
    std::vector<int> pol2_int = polynomial_to_int_array(pol2);

    auto [q , r] = polynomial_division_mod(pol1_int, pol2_int, mod_int);

    std::string output = polynomial_division_to_latex(pol1_int, pol2_int, q, r, mod_int);

    std::cout << output << std::endl;



    return 0;
}
