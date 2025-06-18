add_library(data
  # Intepreter data
  data_node.cpp 
  configuration.cpp 
  game_state_object.cpp

  # Session
  session/manager.cpp
  session/helpers.cpp

  # Game
  game/manager.cpp

  # Misc
  errors.cpp
)

set_target_properties(data PROPERTIES PUBLIC_HEADER ${CMAKE_SOURCE_DIR}/include/data/data.h)

target_include_directories(
  data
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/data>
  $<INSTALL_INTERFACE:${CMAKE_SOURCE_DIR}/include/data>
)

install(TARGETS data 
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)