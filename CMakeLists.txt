# 设置最低的CMake版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(ChatServer)

# 资源文件
file(COPY ${CMAKE_SOURCE_DIR}/root DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/scripts DESTINATION ${CMAKE_BINARY_DIR})

# 源文件
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp
                    ${PROJECT_SOURCE_DIR}/src/utils/*.cpp
                    ${PROJECT_SOURCE_DIR}/src/interpreter/*.cpp
                    ${PROJECT_SOURCE_DIR}/src/interpreter/ASTnodes/*.cpp)

# 添加静态库
file(GLOB_RECURSE LIB_SRC_FILES ${PROJECT_SOURCE_DIR}/src/lib/*.cpp)
add_library(lib ${LIB_SRC_FILES})

# 添加可执行文件
add_executable(chat_server ${SRC_FILES})

# 添加头文件
target_include_directories(chat_server PUBLIC ${PROJECT_SOURCE_DIR}/src)

# 添加链接库
target_compile_features(chat_server PUBLIC cxx_std_20)
target_compile_features(lib PUBLIC cxx_std_20)

target_link_libraries(chat_server lib)
target_link_libraries(lib mysqlcppconn)

#添加测试
enable_testing()
add_subdirectory(tests)