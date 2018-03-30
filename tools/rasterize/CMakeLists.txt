# Rasterize a vector contour image.

# Source files.
set(SRCS
    main.cpp
    rasterize.cpp
    scale.cpp
)

# Header files. Specified in order to get them listed in the IDE.
set(HEADERS
    rasterize.hpp
    scale.hpp
)

# Generate directory groups for IDE.
create_directory_groups(${SRCS} ${HEADERS})

add_executable(rasterize ${SRCS} ${HEADERS})
target_link_libraries(rasterize common stb)

# Require C++11.
set_property(TARGET rasterize PROPERTY CXX_STANDARD 11)
set_property(TARGET rasterize PROPERTY CXX_STANDARD_REQUIRED ON)
