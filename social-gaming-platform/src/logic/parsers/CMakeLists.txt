set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/logic/parsers)

add_library(ts_parser 
  ${SRC_DIR}/Grammar.cpp
  ${SRC_DIR}/tree_sitter/ParsingUtils.cpp
  ${SRC_DIR}/tree_sitter/AssignmentSpecBuilder.cpp
  ${SRC_DIR}/tree_sitter/TSRuleSpecFactory.cpp
  ${SRC_DIR}/tree_sitter/TSParser.cpp
)

target_include_directories(ts_parser 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/logic/parsers
)

CPMAddPackage(
  NAME cpp-tree-sitter
  GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
  GIT_TAG v0.0.3
)

add_grammar_from_repo(tree-sitter-socialgaming
  https://github.com/nsumner/tree-sitter-socialgaming.git
  0.0.2
)

target_link_libraries(ts_parser
  PUBLIC
    tree-sitter-socialgaming
    cpp-tree-sitter
    data
)

target_compile_options(ts_parser PRIVATE -fsanitize=undefined,address)
target_link_options(ts_parser PRIVATE -fsanitize=undefined,address)
