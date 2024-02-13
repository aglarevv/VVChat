cmake_minimum_required(VERSION 3.0)

project(testAI)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件，并将源文件添加到SRC_LIST
aux_source_directory(. SRC_LIST)
add_executable(testAI ${SRC_LIST})

# 链接Curl库
target_link_libraries(testAI oai curl)
