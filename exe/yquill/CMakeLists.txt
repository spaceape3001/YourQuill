add_executable(yquill 
    yquill.cpp 
    yquill.hpp 
    yscanner.cpp
    yserver.cpp
)
target_link_libraries(yquill yq yqdb)
target_include_directories(yquill PRIVATE .)

