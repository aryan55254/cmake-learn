# min cmake version
cmake_minimum_required(VERSION 3.15)

# project and langauge name
project(MiniChronos VERSION 1.0 LANGUAGES CXX)

# enforcing c++ version and turning its extensions off
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find threads library
find_package(Threads REQUIRED)

# target library called chronos_core which has files that are linked to executables 
add_library(chronos_core STATIC
    src/scheduler.cpp
    src/worker.cpp
)

# inlcude directories if not found by compiler find inside /include inside projects source directory
target_include_directories(chronos_core PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

# tells cmake that our chronos_core needs Threads library
target_link_libraries(chronos_core PRIVATE Threads::Threads)

# creates executable
add_executable(chronos_runner src/main.cpp)

# sanatizer on/off
option(ENABLE_ASAN "Enable AddressSanitizer (Memory safety)" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer (Thread safety)" OFF)

# Don't allow both
if(ENABLE_ASAN AND ENABLE_TSAN)
    message(FATAL_ERROR "You cannot enable ASan and TSan simultaneously! Pick one.")
endif()

# Configure ASan
if(ENABLE_ASAN)
    message(STATUS "Build Mode: AddressSanitizer Enabled")
    # Compile flags: Inject the instrumentation
    target_compile_options(chronos_core PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    # Link flags: Link the ASan runtime library
    target_link_options(chronos_core PRIVATE -fsanitize=address)
endif()

# 4. Configure TSan
if(ENABLE_TSAN)
    message(STATUS "Build Mode: ThreadSanitizer Enabled")
    # TSan requires all code (including standard library) to be position independent usually
    target_compile_options(chronos_core PRIVATE -fsanitize=thread)
    target_link_options(chronos_core PRIVATE -fsanitize=thread)
endif()

if(ENABLE_ASAN)
    target_compile_options(chronos_runner PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(chronos_runner PRIVATE -fsanitize=address)
endif()

if(ENABLE_TSAN)
    target_compile_options(chronos_runner PRIVATE -fsanitize=thread)
    target_link_options(chronos_runner PRIVATE -fsanitize=thread)
endif()

# links executable to library
target_link_libraries(chronos_runner PRIVATE chronos_core)

# checks if the compiler is gnu or clang then adds compile time flags for extra warnings and stuff the  first one is generic for all the second one is specificc for debug time and release time difff flags for them 
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(chronos_core PRIVATE
        -Wall
        -Wextra
        -Werror
    )

    target_compile_options(chronos_core PRIVATE
        $<$<CONFIG:Debug>:-O0 -g>
        $<$<CONFIG:Release>:-O3>
    )
endif()
# checks if chronos_native is enabled , detects our cpu and tunes it for specific cpu
option(CHRONOS_NATIVE "Enable native CPU optimizations" OFF)
if(CHRONOS_NATIVE)
    target_compile_options(chronos_core PRIVATE -march=native)
endif()
