CPMAddPackage(
  NAME googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)

enable_testing()

add_executable(tests
  # logic layer tests
  logic/parsers/TestTSParser.cpp
  logic/parsers/TestTSRuleSpecFactory.cpp
  logic/rules/TestAssignmentRule.cpp  
  logic/TestInterpreterState.cpp
  logic/TestInterpreter.cpp
  logic/SchedulerTests.cpp
  
#   data layer tests
  dataNodeTests.cpp
  dataNodeWrapperTests.cpp
)

target_link_libraries(tests
  PRIVATE
    # GTest
    GTest::gmock
    GTest::gtest
    GTest::gtest_main

    # Internal
    logic
    
)

add_executable(external_tests
  external/server/GameServerTest.cpp

)

target_link_libraries(external_tests
  PRIVATE
    # GTest
    GTest::gmock
    GTest::gtest
    GTest::gtest_main

    # Internal
    nlohmann_json
    tools
    logic
)

target_compile_options(tests PRIVATE -fsanitize=undefined,address)
target_link_options(tests PRIVATE -fsanitize=undefined,address)

add_test(NAME tests
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests
)