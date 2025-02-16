cmake_minimum_required(VERSION 3.10)
project(minishell LANGUAGES C)

# Automatically find all .c files
file(GLOB_RECURSE SOURCES src/*.c)

# Add executable
add_executable(minishell ${SOURCES})

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable clang-tidy checks
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-*,misc-unused-parameters;-fix")

# Include directories
include_directories(inc libft/inc)

# Link libraries
set(LIBFT_PATH "${CMAKE_SOURCE_DIR}/libft/libft.a")

# Properly link Libft as a static library
add_library(libft STATIC IMPORTED)
set_target_properties(libft PROPERTIES IMPORTED_LOCATION ${LIBFT_PATH})
target_link_libraries(minishell libft readline)
