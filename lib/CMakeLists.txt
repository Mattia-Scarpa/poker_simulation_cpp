#----------------------------------------------------------------------

SET(INCLUDE_LIB_SRC player.cpp board.cpp deck.cpp hand.cpp combinations.cpp card.cpp)

add_library(lib ${INCLUDE_LIB_SRC})

target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
