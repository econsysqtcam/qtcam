cmake_minimum_required(VERSION 3.10)
project(DenoiseProject LANGUAGES CXX)

#---------------------------------------
# Compiler setup
#---------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Optimization and architecture flags
add_compile_options(-O3 -march=native -fopenmp)

# Output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#---------------------------------------
# Denoise Shared Library
#---------------------------------------
add_library(Denoise SHARED
    Denoise.cpp
    DenoiseCls.cpp
    Denoise_LUT.cpp
)

target_include_directories(Denoise
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link external libraries to Denoise
target_link_libraries(Denoise
    PUBLIC
        m              # libm (math)
        fftw3f          # single-precision FFTW3
        OpenMP::OpenMP_CXX
)

#---------------------------------------
# Main Executable
#---------------------------------------
add_executable(main
    main.cpp
)

target_link_libraries(main
    PRIVATE
        Denoise
        m
        fftw3f
        OpenMP::OpenMP_CXX
)

#---------------------------------------
# Find OpenMP (portable way)
#---------------------------------------
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Denoise PUBLIC OpenMP::OpenMP_CXX)
    target_link_libraries(main PRIVATE OpenMP::OpenMP_CXX)
endif()

#---------------------------------------
# Optional: install step
#---------------------------------------
install(TARGETS Denoise main
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

