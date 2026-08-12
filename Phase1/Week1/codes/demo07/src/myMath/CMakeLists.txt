add_library(myMath STATIC)

target_sources(myMath
    PRIVATE 
        myMath.cpp
)

target_include_directories(myMath
    PUBLIC 
        ${INCLUDE_DIR}
)

target_compile_features(myMath
    PUBLIC 
        cxx_std_17
)