CPMAddPackage(
  NAME json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2 
)

add_subdirectory(src)
add_subdirectory(tools)

add_executable(privateServer
  privateServer.cpp
)

set_target_properties(privateServer
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 23
                      PREFIX ""
)

find_package(Threads REQUIRED)

target_link_libraries(privateServer
  tools
  ${CMAKE_THREAD_LIBS_INIT}
  data
  logic
  -fsanitize=address
  -fsanitize=undefined
)

install(TARGETS privateServer
  RUNTIME DESTINATION bin
)