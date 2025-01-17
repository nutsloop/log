cmake_minimum_required(VERSION 3.30)
project(log)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# CMAKE_BINARY_DIR should be build
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(INSTALL_DIR /usr/local/bin)

# Collect include directories recursively
file(GLOB_RECURSE HEADER_DIRS LIST_DIRECTORIES true "${INCLUDE_DIR}/*")

# Initialize the include directories list with the root
set(INCLUDE_DIRS ${INCLUDE_DIR})

# Add only directories to INCLUDE_DIRS
foreach (DIR ${HEADER_DIRS})
    if (IS_DIRECTORY "${DIR}")
        list(APPEND INCLUDE_DIRS "${DIR}")
    endif ()
endforeach ()

# Source files
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.c++)

# Set output directories for executables and libraries
# Define output directories for executables and libraries
# The ${CMAKE_BINARY_DIR} changes dynamically based on the build directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Optimization flags
set(OPT_FLAGS "-O3 -march=native -flto")
set(DEBUG_FLAGS "-g -O0 -fno-omit-frame-pointer")
set(RELEASE_FLAGS "${OPT_FLAGS} -DNDEBUG -DDEBUG_LOG=false")
set(RELWITHDEBINFO_FLAGS "${OPT_FLAGS} -g -DNDEBUG")
set(EXTRA_FLAGS "-fexperimental-library")

# Combine flags for each build type
set(CMAKE_CXX_FLAGS_DEBUG "${DEBUG_FLAGS} ${EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${RELEASE_FLAGS} ${EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${RELWITHDEBINFO_FLAGS} ${EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${RELEASE_FLAGS} -Os ${EXTRA_FLAGS}")

# Output some debugging information
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL: ${CMAKE_CXX_FLAGS_MINSIZEREL}")

# Add the library
add_library(${PROJECT_NAME} log.h++ ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

# Add the executable for testing purposes only in debug mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(_log main.c++)
    target_link_libraries(_log PRIVATE ${PROJECT_NAME})
endif ()
