ADD_EXECUTABLE(apiCallTest apiCallTest.cpp)

target_link_libraries(apiCallTest tools curlpp jsoncpp_lib )
target_include_directories(apiCallTest PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/curlpp/include"
                          "${PROJECT_SOURCE_DIR}/jsoncpp/include"
                          "${PROJECT_SOURCE_DIR}/jsoncpp/include/json"
                          "${JSON_INC_PATH}"
                          )


ADD_TEST(apiCall apiCallTest)

