cmake_minimum_required(VERSION 3.1.0)

project(PhotoGeo)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Compiler-specific configuration.
if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O1")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# This function should be passed a list of all files in a target. It will automatically generate
# file groups following the directory hierarchy, so that the layout of the files in IDEs matches the
# one in the filesystem.
function(create_directory_groups)
    # Place any files that aren't in the source list in a separate group so that they don't get in
    # the way.
    source_group("Other Files" REGULAR_EXPRESSION ".")

    foreach(file_name ${ARGV})
        get_filename_component(dir_name "${file_name}" PATH)
        # Group names use '\' as a separator even though the entire rest of CMake uses '/'...
        string(REPLACE "/" "\\" group_name "${dir_name}")
        source_group("${group_name}" FILES "${file_name}")
    endforeach()
endfunction()

# Options.
option(BUILD_SHARED_LIBS "Build libraries as shared." OFF)

# Dependencies.
add_subdirectory(externals)

# Handle options.
if(BUILD_SHARED_LIBS)
    add_definitions(-DBUILD_SHARED)
endif()

# Library.
add_subdirectory(src)

# Tools.
add_subdirectory(tools)
