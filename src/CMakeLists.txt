add_subdirectory(lib)

add_executable(server server.cpp)
target_link_libraries(server packet tools response player curlpp jsoncpp_lib)

target_include_directories(server PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/curlpp/include"
                          "${PROJECT_SOURCE_DIR}/jsoncpp/include"
                          "${PROJECT_SOURCE_DIR}/jsoncpp/include/json"
                          "${JSON_INC_PATH}"
                          )