add_subdirectory(parsers)


set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/logic)

add_library(logic
  # rules depend on InterpreterState
  ${SRC_DIR}/interpreter/InterpreterState.cpp 
  
  # compile rules
  ${SRC_DIR}/rules/AssignmentRule.cpp 

  ${SRC_DIR}/interpreter/Interpreter.cpp
  ${SRC_DIR}/GameProcess.cpp
)

target_include_directories(logic 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/logic
        ${CMAKE_SOURCE_DIR}/include/logic/interpreter
        ${CMAKE_SOURCE_DIR}/include/logic/parsers
        ${CMAKE_SOURCE_DIR}/include/logic/rules
        ${CMAKE_SOURCE_DIR}/include/logic/scheduler
)

target_link_libraries(logic
  PUBLIC
    data
    ts_parser
)

target_compile_options(logic PRIVATE -fsanitize=undefined,address)
target_link_options(logic PRIVATE -fsanitize=undefined,address)
