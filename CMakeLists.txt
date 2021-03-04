cmake_minimum_required (VERSION 3.0)
project (Tetris)

##############
## EXTERNAL ##
##############
add_subdirectory(external/SFML)

##############
## PROJECT ##
##############

include_directories(Include)

add_executable(${PROJECT_NAME} Sources/main.cpp Sources/gameClass.cpp Sources/network.cpp)

target_link_libraries(${PROJECT_NAME} sfml-window sfml-graphics sfml-system sfml-audio sfml-network)

target_include_directories(${PROJECT_NAME}
  SYSTEM PUBLIC
  external/SFML/include/
)

##############
## INSTALL ##
##############

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/output)
        
install(TARGETS ${PROJECT_NAME} sfml-window sfml-graphics sfml-system sfml-audio sfml-network)