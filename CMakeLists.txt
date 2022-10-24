#-------------------------------------------------------------------------
# CMakeList.txt: CMake Project

cmake_minimum_required (VERSION 3.8)

project ("poker_simulation")

# Generating the executable 
add_executable (simulate src/simulate.cpp)
add_executable (estimate src/estimate.cpp)

# Adding classes folder
add_subdirectory(include)
find_package(Threads)

# Adding properties
set_target_properties(include PROPERTIES LINKER_LANGUAGE CXX)


target_link_libraries(simulate
                        include
                        ${CMAKE_THREAD_LIBS_INIT}
)
target_link_libraries(estimate
                        include
                        ${CMAKE_THREAD_LIBS_INIT}
)



