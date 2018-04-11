# Rasterize a vector contour image.

# Source files.
set(SRCS
    main.cpp
    rasterize.cpp
    read_svg.cpp
    scale.cpp
    vertex_count.cpp
)

# Header files. Specified in order to get them listed in the IDE.
set(HEADERS
    rasterize.hpp
    read_svg.hpp
    scale.hpp
    vertex_count.hpp
)

# Generate directory groups for IDE.
create_directory_groups(${SRCS} ${HEADERS})

add_executable(rasterize ${SRCS} ${HEADERS})
target_link_libraries(rasterize photogeo stb tinyxml2)

# Require C++11.
set_property(TARGET rasterize PROPERTY CXX_STANDARD 11)
set_property(TARGET rasterize PROPERTY CXX_STANDARD_REQUIRED ON)
