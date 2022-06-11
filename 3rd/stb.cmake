add_library(stb SHARED 
    stb.cpp
)

target_include_directories(stb PUBLIC ./stb)
    
LinkTest(stb)


