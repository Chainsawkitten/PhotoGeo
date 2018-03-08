# Rasterize a vector contour image.

# Source files.
set(SRCS
    main.cpp
)

# Header files. Specified in order to get them listed in the IDE.
set(HEADERS
    
)

# Generate directory groups for IDE.
create_directory_groups(${SRCS} ${HEADERS})

add_executable(rasterize ${SRCS} ${HEADERS})

# Require C++11.
set_property(TARGET rasterize PROPERTY CXX_STANDARD 11)
set_property(TARGET rasterize PROPERTY CXX_STANDARD_REQUIRED ON)
