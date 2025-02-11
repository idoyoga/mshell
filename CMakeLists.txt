cmake_minimum_required(VERSION 3.10)
project(minishell LANGUAGES C)

set(SOURCES
	src/alloc.c
	src/alloc_utils.c
	src/env_utils.c
	src/init.c
	src/main.c
)

# Include directories
include_directories(inc)
include_directories(src)
include_directories(libft/inc)

# Link with libft
set(LIBFT_PATH "${CMAKE_SOURCE_DIR}/libft/libft.a")  # Absolute path to libft.a
add_executable(minishell ${SOURCES})

# Link with libft.a (static library)
target_link_libraries(minishell ${LIBFT_PATH})

target_link_libraries(minishell readline) # Link readline
