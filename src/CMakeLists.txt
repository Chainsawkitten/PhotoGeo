# PhotoGeo library.

# Source files.
set(SRCS
    photogeo.cpp
)

# Header files. Specified in order to get them listed in the IDE.
set(HEADERS
    ../include/photogeo.h
)

# Generate directory groups for IDE.
create_directory_groups(${SRCS} ${HEADERS})

add_library(photogeo ${SRCS} ${HEADERS})
target_link_libraries(photogeo opencv)
target_include_directories(photogeo PUBLIC ../include)
