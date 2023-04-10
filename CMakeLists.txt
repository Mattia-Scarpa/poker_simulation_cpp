#-------------------------------------------------------------------------
# CMakeList.txt: CMake Project

cmake_minimum_required(VERSION 3.12)

#set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(PROJECT_NAME poker_simulation)
project (${PROJECT_NAME}$ VERSION 0.3)

# set build type on Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()


# Generating the executable 
add_executable (simulate src/main_game.cpp)
#add_executable (estimate src/estimate.cpp)

# Adding classes folder
include_directories(include)
add_subdirectory(lib)

find_package(Threads)
find_package(OpenMP REQUIRED)


if(OpenMP_CXX_FOUND)
    target_compile_options(simulate PUBLIC -fopenmp)
endif()

# Adding properties
# set_target_properties(include PROPERTIES LINKER_LANGUAGE CXX)


target_link_libraries(simulate
                        PUBLIC lib
                        ${CMAKE_THREAD_LIBS_INIT}
                        OpenMP::OpenMP_CXX
)
#target_link_libraries(estimate
#                        include
#                        ${CMAKE_THREAD_LIBS_INIT}
#)

# Imposta le opzioni di compilazione
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
