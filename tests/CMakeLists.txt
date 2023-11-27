add_executable(test_log test_log.cpp
                        ${CMAKE_SOURCE_DIR}/src/log.cpp)

target_include_directories(test_log PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_log PRIVATE cxx_std_20)

add_test(NAME test_log COMMAND test_log)