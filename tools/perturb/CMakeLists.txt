# Perturb a raster image.

# Source files.
set(SRCS
    main.cpp
    perturb.cpp
)

# Header files. Specified in order to get them listed in the IDE.
set(HEADERS
    perturb.hpp
)

# Copy data.
add_custom_target(PerturbData ALL)
add_custom_command(
    TARGET PerturbData
    COMMAND ${CMAKE_COMMAND} -E copy_directory data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/perturb_data
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying perturb data."
)

# Generate directory groups for IDE.
create_directory_groups(${SRCS} ${HEADERS})

add_executable(perturb ${SRCS} ${HEADERS})
target_link_libraries(perturb opencv stb)

# Require C++11.
set_property(TARGET perturb PROPERTY CXX_STANDARD 11)
set_property(TARGET perturb PROPERTY CXX_STANDARD_REQUIRED ON)
