cmake_minimum_required(VERSION 3.5)

project(minecraft_server)

# Configure wether libraries will be static or shared linked
set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(curlpp)
add_subdirectory(jsoncpp)

add_subdirectory(src)

enable_testing()

add_subdirectory(tests)