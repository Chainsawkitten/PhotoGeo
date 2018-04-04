# PhotoGeo commandline tool.

# Source files.
set(SRCS
    conversion.cpp
    main.cpp
    png.cpp
    profiling.cpp
    svg.cpp
)

# Header files. Specified in order to get them listed in the IDE.
set(HEADERS
    conversion.hpp
    png.hpp
    profiling.hpp
    svg.hpp
)

# Generate directory groups for IDE.
create_directory_groups(${SRCS} ${HEADERS})

add_executable(PhotoGeoCmd ${SRCS} ${HEADERS})
target_link_libraries(PhotoGeoCmd common stb)

# Require C++11.
set_property(TARGET PhotoGeoCmd PROPERTY CXX_STANDARD 11)
set_property(TARGET PhotoGeoCmd PROPERTY CXX_STANDARD_REQUIRED ON)
