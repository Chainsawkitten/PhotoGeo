# PhotoGeo library.

# Source files.
set(SRCS
    photogeo.cpp
    quantization/color_conversion.cpp
    quantization/quantization.cpp
    tracing/marching_squares.cpp
)

# Header files. Specified in order to get them listed in the IDE.
set(HEADERS
    ../include/photogeo.h
    quantization/color_conversion.hpp
    quantization/quantization.hpp
    tracing/marching_squares.hpp
)

# Generate directory groups for IDE.
create_directory_groups(${SRCS} ${HEADERS})

add_library(photogeo ${SRCS} ${HEADERS})
target_link_libraries(photogeo opencv)
target_include_directories(photogeo PUBLIC ../include)

# Require C++11.
set_property(TARGET photogeo PROPERTY CXX_STANDARD 11)
set_property(TARGET photogeo PROPERTY CXX_STANDARD_REQUIRED ON)
