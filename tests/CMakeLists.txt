
#test log
add_executable(test_log test_log.cpp
                        ${CMAKE_SOURCE_DIR}/src/log.cpp)

target_include_directories(test_log PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_log PRIVATE cxx_std_20)

add_test(NAME test_log COMMAND test_log)


#test thread pool
add_executable(test_thread_pool test_threadpool.cpp
                                ${CMAKE_SOURCE_DIR}/src/threadpool.cpp)

target_include_directories(test_thread_pool PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_thread_pool PRIVATE cxx_std_20)

add_test(NAME test_thread_pool COMMAND test_thread_pool)

#test db connection pool

add_executable(test_db_connectionpool test_db_connectionpool.cpp
                                    ${CMAKE_SOURCE_DIR}/src/db_connectionpool.cpp)

target_include_directories(test_db_connectionpool PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_db_connectionpool PRIVATE cxx_std_20)

target_link_libraries(test_db_connectionpool mysqlcppconn)

add_test(NAME test_db_connectionpool COMMAND test_db_connectionpool)

#test httprequest
add_executable(test_httprequest test_http_request.cpp
                                ${CMAKE_SOURCE_DIR}/src/http_request.cpp
                                ${CMAKE_SOURCE_DIR}/src/log.cpp
                                ${CMAKE_SOURCE_DIR}/src/buffer.cpp)

target_include_directories(test_httprequest PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_httprequest PRIVATE cxx_std_20)

add_test(NAME test_httprequest COMMAND test_httprequest)

#test buffer

add_executable(test_buffer test_buffer.cpp
                            ${CMAKE_SOURCE_DIR}/src/buffer.cpp)

target_include_directories(test_buffer PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_buffer PRIVATE cxx_std_20)

add_test(NAME test_buffer COMMAND test_buffer)