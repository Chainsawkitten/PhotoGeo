# Common functionality used by the different tools.

# Source files.
set(SRCS
    read_svg.cpp
)

# Header files. Specified in order to get them listed in the IDE.
set(HEADERS
    read_svg.hpp
)

# Generate directory groups for IDE.
create_directory_groups(${SRCS} ${HEADERS})

add_library(common ${SRCS} ${HEADERS})
target_link_libraries(common photogeo tinyxml2)
target_include_directories(common PUBLIC .)

# Require C++11.
set_property(TARGET common PROPERTY CXX_STANDARD 11)
set_property(TARGET common PROPERTY CXX_STANDARD_REQUIRED ON)
