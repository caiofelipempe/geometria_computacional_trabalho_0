cmake_minimum_required(VERSION 3.16)
project(convex_hull_3d)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLM
# find_package(glm CONFIG REQUIRED)

# Find CGAL
find_package(CGAL REQUIRED)

# Collect all source files except main.cpp
file(GLOB_RECURSE SOURCES
    "app/src/private/*.cpp"
    "app/src/public/*.cpp"
    "app/src/private/*.c"
    "app/src/public/*.c"
)
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/app/main.cpp")

# Collect public headers
file(GLOB_RECURSE PUBLIC_HEADERS
    "app/src/public/*.hpp"
    "app/src/public/*.h"
)

# Collect private headers
file(GLOB_RECURSE PRIVATE_HEADERS
    "app/src/private/*.hpp"
    "app/src/private/*.h"
)

# Create library
add_library(${PROJECT_NAME}_lib STATIC ${SOURCES} ${PUBLIC_HEADERS} ${PRIVATE_HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME}_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/app/src/public
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/app/src/private
)

# Link libraries to lib
target_link_libraries(${PROJECT_NAME}_lib
    PUBLIC
        SDL2::SDL2
        SDL2::SDL2main
        OpenGL::GL
        OpenGL::GLU
        CGAL::CGAL
)

# Add executable
add_executable(${PROJECT_NAME} app/main.cpp)

# Link executable to lib
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)

# Install public headers
install(DIRECTORY app/src/public/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)