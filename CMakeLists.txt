cmake_minimum_required(VERSION 3.30)
project(log)

option(USE_RELATIVE_PATHS "Use relative paths for libraries and includes" OFF)
# Define an option to toggle between static and shared libraries
option(BUILD_SHARED_LIBS "Build a shared library" OFF)
option(BUILD_MAIN_EXE "Build the main executable include in the repository for testing purpose" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure based on the switch
if(USE_RELATIVE_PATHS)
  set(ANSI_ROOT_INCLUDE_DIR "../ansi/include")
else()
  set(ANSI_ROOT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/deps/ansi/include")
endif()

# Root include directory
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include ${ANSI_ROOT_INCLUDE_DIR})

# Source files
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.c++)

# Optimization flags
set(OPT_FLAGS "-O3 -march=native -flto")
set(DEBUG_FLAGS "-g -O0 -fno-omit-frame-pointer -DDEBUG_LOG=true -DEXPERIMENTAL_INSTANCE=true")
set(RELEASE_FLAGS "${OPT_FLAGS} -DNDEBUG -DDEBUG_LOG=false -DEXPERIMENTAL_INSTANCE=true")
set(RELWITHDEBINFO_FLAGS "${OPT_FLAGS} -g -DNDEBUG")
set(EXTRA_FLAGS "-fexperimental-library")

# Combine flags for each build type
set(CMAKE_CXX_FLAGS_DEBUG "${DEBUG_FLAGS} ${EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${RELEASE_FLAGS} ${EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${RELWITHDEBINFO_FLAGS} ${EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${RELEASE_FLAGS} -Os ${EXTRA_FLAGS}")

# Add the library
add_library(${PROJECT_NAME} include/log.h++ ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

# Optional: Add SO versioning for shared libraries
if (BUILD_SHARED_LIBS)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 0.0.3
    SOVERSION 0
  )
endif ()

# Add the executable for testing purposes if BUILD_MAIN_EXE is set to ON
if (BUILD_MAIN_EXE)
  add_executable(_log main.c++)
  target_link_libraries(_log PRIVATE ${PROJECT_NAME})
endif ()
