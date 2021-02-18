cmake_minimum_required (VERSION 3.0)
project (Tetris)

##############
## EXTERNAL ##
##############
add_subdirectory(external/SFML)

##############
## PROJECT ##
##############

include_directories(IN204/include)

add_executable(tetris IN204/Sources/main.cpp IN204/Sources/gameClass.cpp IN204/Sources/network.cpp)

target_link_libraries(tetris sfml-window sfml-graphics sfml-system sfml-audio sfml-network)

target_include_directories(tetris
  SYSTEM PUBLIC
  external/SFML/include/
)

##############
## INSTALL ##
##############

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin)
        
install(TARGETS tetris sfml-window sfml-graphics sfml-system sfml-audio sfml-network
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin
        )