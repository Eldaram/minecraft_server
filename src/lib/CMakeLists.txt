# packet library
add_library(packet packet.cpp)
target_include_directories(packet PUBLIC includes)
set_target_properties(packet PROPERTIES PUBLIC_HEADER includes/packet.h)
set_target_properties(packet PROPERTIES SOVERSION 1)
target_link_libraries(packet curlpp jsoncpp_lib)
target_include_directories(packet PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/curlpp/include"
                          "${PROJECT_SOURCE_DIR}/jsoncpp/include"
                          "${PROJECT_SOURCE_DIR}/jsoncpp/include/json"
                          "${JSON_INC_PATH}"
                          )

# tools library
add_library(tools tools.cpp)
target_include_directories(tools PUBLIC includes)
set_target_properties(tools PROPERTIES PUBLIC_HEADER includes/tools.h)
set_target_properties(tools PROPERTIES SOVERSION 1)
target_link_libraries(tools curlpp jsoncpp_lib)
target_include_directories(tools PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/curlpp/include"
                          "${PROJECT_SOURCE_DIR}/jsoncpp/include"
                          "${PROJECT_SOURCE_DIR}/jsoncpp/include/json"
                          "${JSON_INC_PATH}"
                          )

# response library
add_library(response response.cpp)
target_include_directories(response PUBLIC includes)
set_target_properties(response PROPERTIES PUBLIC_HEADER includes/response.h)
set_target_properties(response PROPERTIES SOVERSION 1)
target_link_libraries(response curlpp jsoncpp_lib)
target_include_directories(response PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/curlpp/include"
                          "${PROJECT_SOURCE_DIR}/jsoncpp/include"
                          "${PROJECT_SOURCE_DIR}/jsoncpp/include/json"
                          "${JSON_INC_PATH}"
                          )

# player library
add_library(player player.cpp)
target_include_directories(player PUBLIC includes)
set_target_properties(player PROPERTIES PUBLIC_HEADER includes/player.h)
set_target_properties(player PROPERTIES SOVERSION 1)

install(TARGETS packet tools response player
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include
)