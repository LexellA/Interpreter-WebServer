# 设置最低的CMake版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(ChatServer)


file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(chat_server ${SRC_FILES})

target_include_directories(chat_server PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_compile_features(chat_server PUBLIC cxx_std_20)

target_link_libraries(chat_server mysqlcppconn)

#添加测试
enable_testing()
add_subdirectory(tests)