cmake_minimum_required(VERSION 3.10)

project(jauntlet)

# Set the directories for source code
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(JAUNTLET_DIR "${CMAKE_SOURCE_DIR}/Jauntlet")

# Get a list of all .cpp source files in the "src" directory
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")

# Get a list of all .cpp source files in the "Jauntlet" directory
file(GLOB_RECURSE JAUNTLET_FILES "${JAUNTLET_DIR}/*.cpp")

# Set the output directory for libraries
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/dependencies/lib")

# Use pkg-config to find SDL2
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Use pkg-config to find other dependencies, e.g., GLEW and OpenGL
pkg_check_modules(GLEW REQUIRED glew)
pkg_check_modules(OPENGL REQUIRED gl)

# Use pkg-config to find FreeType
pkg_check_modules(FREETYPE REQUIRED freetype2)

# Create a library from the "Jauntlet" source files
add_library(jauntlet ${JAUNTLET_FILES})

# Specify include directories for the "Jauntlet" library
# target_include_directories(jauntlet PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/include")

# Create an executable from the "src" source files
add_executable(scoundrills ${SRC_FILES})

if(WIN32)
    # On Windows, use MSVC
    set(CMAKE_CXX_COMPILER "cl")
elseif(UNIX)
    # On Unix, use g++
    set(CMAKE_CXX_COMPILER "g++")
endif()

# On all platforms, link the jauntlet library with our executable.
target_link_libraries(scoundrills PRIVATE jauntlet)

if(WIN32)
    # On Windows, link with .lib files in [project root]/dependencies/lib/
    file(GLOB WIN_LIB_FILES "${CMAKE_SOURCE_DIR}/dependencies/lib/*.lib")
    target_link_libraries(scoundrills PRIVATE ${WIN_LIB_FILES})
elseif(UNIX)
    # On Unix, use dependencies from pkg-config
    target_link_libraries(scoundrills PRIVATE ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES})
endif()

# Specify include directories for the "Jauntlet" directory (outside of it)
target_include_directories(scoundrills PRIVATE "${CMAKE_SOURCE_DIR}")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
