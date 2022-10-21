#----------------------------------------------------------------------

SET(INCLUDE_LIB_SRC poker.cpp) # index52c7.cpp simulate.h

add_library(include ${INCLUDE_LIB_SRC})

target_include_directories(include PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
