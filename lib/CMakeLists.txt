#----------------------------------------------------------------------

SET(INCLUDE_LIB_SRC card.cpp hand.cpp deck.cpp combinations.cpp board.cpp) #

add_library(lib ${INCLUDE_LIB_SRC})

target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
