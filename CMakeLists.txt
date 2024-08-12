cmake_minimum_required(VERSION 3.28)
project(congruence-class-calculator)

set(CMAKE_CXX_STANDARD 17)

add_executable(congruence-class-calculator main.cpp)

add_subdirectory(libCongruenceClass)
add_subdirectory(libLaTeXConverter)
add_subdirectory(libStringUtils)
add_subdirectory(libPolynomialOperations)


add_subdirectory(libPolynomial)

target_link_libraries(congruence-class-calculator libCongruenceClass)
target_link_libraries(congruence-class-calculator libLaTeXConverter)
target_link_libraries(congruence-class-calculator libStringUtils)
target_link_libraries(congruence-class-calculator libPolynomialOperations)

target_link_libraries(congruence-class-calculator Polynomial)

