#-------------------------------------------------------------------------
# CMakeList.txt: CMake Project

cmake_minimum_required (VERSION 3.8)

project ("poker_simulation")

# Generating the executable 
add_executable (simulate src/core.cpp)

# Adding classes folder
add_subdirectory(include)
find_package(Threads)

# Adding properties
set_target_properties(include PROPERTIES LINKER_LANGUAGE CXX)


target_link_libraries(simulate
                        include
                        ${CMAKE_THREAD_LIBS_INIT}
)



