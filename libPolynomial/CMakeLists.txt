add_library(Polynomial Monomial.h Polynomial.h
        PolynomialDivision.h)
target_include_directories(Polynomial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(Polynomial PROPERTIES LINKER_LANGUAGE CXX)