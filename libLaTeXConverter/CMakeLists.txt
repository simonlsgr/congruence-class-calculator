add_library(libLaTeXConverter PolynomialConversionToLatex.h)
target_include_directories(libLaTeXConverter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(libLaTeXConverter PROPERTIES LINKER_LANGUAGE CXX)