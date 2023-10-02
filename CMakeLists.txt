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

# Find and configure OpenGL with GLVND preference
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Find and configure SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add SDL2_mixer library
find_package(SDL2_mixer REQUIRED)
include_directories(${SDL_MIXER_INCLUDE_DIRS})

# Find and configure GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# Find and configure GLM
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# Find and configure FreeType
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

# Create a library from the "Jauntlet" source files
add_library(jauntlet ${JAUNTLET_FILES})

# Specify include directories for the "Jauntlet" library
target_include_directories(jauntlet PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/include")

# Create an executable from the "src" source files
add_executable(game ${SRC_FILES})

if(WIN32)
    # On Windows, use MSVC
    set(CMAKE_CXX_COMPILER "cl")
elseif(UNIX)
    # On Unix, use g++
    set(CMAKE_CXX_COMPILER "g++")
endif()

# On all platforms, link the jauntlet library with our executable.
target_link_libraries(game PRIVATE jauntlet)

if(WIN32)
    # On Windows, link with .lib files in [project root]/dependencies/lib/
    file(GLOB WIN_LIB_FILES "${CMAKE_SOURCE_DIR}/dependencies/lib/*.lib")
    target_link_libraries(game PRIVATE ${WIN_LIB_FILES})
elseif(UNIX)
    # On Unix, use SDL2::SDL2, GLEW::GLEW, OpenGL::OpenGL, and Freetype::Freetype
    target_link_libraries(game PRIVATE SDL2::SDL2 SDL2_mixer::SDL2_mixer GLEW::GLEW OpenGL::OpenGL Freetype::Freetype)
endif()

# Specify include directories for the "Jauntlet" directory (outside of it)
target_include_directories(game PRIVATE "${CMAKE_SOURCE_DIR}")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
