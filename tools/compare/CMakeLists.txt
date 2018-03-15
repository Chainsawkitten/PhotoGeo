# Compare two silhouette images.

# Source files.
set(SRCS
    main.cpp
)

# Header files. Specified in order to get them listed in the IDE.
set(HEADERS
    
)

# Generate directory groups for IDE.
create_directory_groups(${SRCS} ${HEADERS})

add_executable(compare ${SRCS} ${HEADERS})
target_link_libraries(compare common stb)

# Require C++11.
set_property(TARGET compare PROPERTY CXX_STANDARD 11)
set_property(TARGET compare PROPERTY CXX_STANDARD_REQUIRED ON)
