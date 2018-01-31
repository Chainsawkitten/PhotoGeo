# PhotoGeo commandline tool.

# Source files.
set(SRCS
    main.cpp
)

# Header files. Specified in order to get them listed in the IDE.
set(HEADERS

)

# Generate directory groups for IDE.
create_directory_groups(${SRCS} ${HEADERS})

add_executable(PhotoGeoCmd ${SRCS} ${HEADERS})
target_link_libraries(PhotoGeoCmd photogeo)
