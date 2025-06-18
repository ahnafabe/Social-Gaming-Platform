add_library(tools 
  GameServer.cpp
  Request.cpp
  Response.cpp
  RequestHandler.cpp
)

target_include_directories(tools
 PUBLIC
  ${CMAKE_SOURCE_DIR}/include/external
)

target_link_libraries(tools 
 PUBLIC
  nlohmann_json::nlohmann_json # JSON library
  networking                   # Networking 
  logic                        # logic layer
  -fsanitize=address
  -fsanitize=undefined
)

target_compile_features(tools 
                        PUBLIC 
                        cxx_std_23
)
