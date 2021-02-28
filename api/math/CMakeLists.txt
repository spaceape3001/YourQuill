set(math_sources
    ./Fraction.cpp
    ./Fraction.hpp
    ./MathFwd.hpp
)

add_library(yqmath SHARED ${math_sources})
target_link_libraries(yqmath PUBLIC yqmeta yqutil)

