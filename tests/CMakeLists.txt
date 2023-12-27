
#test log
add_executable(test_log test_log.cpp
                        ${CMAKE_SOURCE_DIR}/src/lib/log.cpp)

target_include_directories(test_log PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_log PRIVATE cxx_std_20)

add_test(NAME test_log COMMAND test_log)


#test thread pool
add_executable(test_thread_pool test_threadpool.cpp
                                ${CMAKE_SOURCE_DIR}/src/lib/threadpool.cpp)

target_include_directories(test_thread_pool PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_thread_pool PRIVATE cxx_std_20)

add_test(NAME test_thread_pool COMMAND test_thread_pool)

#test db connection pool

add_executable(test_db_connectionpool test_db_connectionpool.cpp
                                    ${CMAKE_SOURCE_DIR}/src/lib/db_connectionpool.cpp)

target_include_directories(test_db_connectionpool PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_db_connectionpool PRIVATE cxx_std_20)

target_link_libraries(test_db_connectionpool mysqlcppconn)

add_test(NAME test_db_connectionpool COMMAND test_db_connectionpool)

#test httprequest
add_executable(test_httprequest test_http_request.cpp
                                ${CMAKE_SOURCE_DIR}/src/lib/http_request.cpp
                                ${CMAKE_SOURCE_DIR}/src/lib/log.cpp
                                ${CMAKE_SOURCE_DIR}/src/lib/buffer.cpp)

target_include_directories(test_httprequest PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_httprequest PRIVATE cxx_std_20)

add_test(NAME test_httprequest COMMAND test_httprequest)

#test buffer

add_executable(test_buffer test_buffer.cpp
                            ${CMAKE_SOURCE_DIR}/src/lib/buffer.cpp)

target_include_directories(test_buffer PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_buffer PRIVATE cxx_std_20)

add_test(NAME test_buffer COMMAND test_buffer)

#test httpresponse

add_executable(test_httpresponse test_http_response.cpp
                                ${CMAKE_SOURCE_DIR}/src/lib/http_response.cpp
                                ${CMAKE_SOURCE_DIR}/src/lib/log.cpp
                                ${CMAKE_SOURCE_DIR}/src/lib/buffer.cpp)

target_include_directories(test_httpresponse PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_httpresponse PRIVATE cxx_std_20)

file(COPY ${CMAKE_SOURCE_DIR}/root/ DESTINATION ${CMAKE_BINARY_DIR}/tests/root/)

add_test(NAME test_httpresponse COMMAND test_httpresponse)

#test httpconnection

add_executable(test_httpconnection test_http_connection.cpp
                                    ${CMAKE_SOURCE_DIR}/src/lib/http_connection.cpp
                                    ${CMAKE_SOURCE_DIR}/src/lib/http_request.cpp
                                    ${CMAKE_SOURCE_DIR}/src/lib/http_response.cpp
                                    ${CMAKE_SOURCE_DIR}/src/lib/log.cpp
                                    ${CMAKE_SOURCE_DIR}/src/lib/buffer.cpp)

target_include_directories(test_httpconnection PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_httpconnection PRIVATE cxx_std_20)

add_test(NAME test_httpconnection COMMAND test_httpconnection)



#test timer
add_executable(test_timer test_timer.cpp
                            ${CMAKE_SOURCE_DIR}/src/lib/timer.cpp
                            ${CMAKE_SOURCE_DIR}/src/lib/log.cpp)

target_include_directories(test_timer PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_timer PRIVATE cxx_std_20)

add_test(NAME test_timer COMMAND test_timer)

#test intepreter

file(GLOB_RECURSE INTEPRETER_SRC ${CMAKE_SOURCE_DIR}/src/interpreter/*.cpp
                                ${CMAKE_SOURCE_DIR}/src/interpreter/ASTnodes/*.cpp)

file(COPY ${CMAKE_SOURCE_DIR}/tests/test_scripts DESTINATION ${CMAKE_BINARY_DIR}/tests/ )

add_executable(test_intepreter test_intepreter.cpp
                                ${INTEPRETER_SRC})

target_include_directories(test_intepreter PRIVATE ${CMAKE_SOURCE_DIR}/src/interpreter
                                                   ${CMAKE_SOURCE_DIR}/src/interpreter/ASTnodes)

target_compile_features(test_intepreter PRIVATE cxx_std_20)

add_test(NAME test_intepreter COMMAND test_intepreter)


#test router

add_executable(test_router test_router.cpp
                            ${CMAKE_SOURCE_DIR}/src/lib/router.cpp
                            ${CMAKE_SOURCE_DIR}/src/lib/http_request.cpp
                            ${CMAKE_SOURCE_DIR}/src/lib/http_response.cpp
                            ${CMAKE_SOURCE_DIR}/src/lib/log.cpp
                            ${CMAKE_SOURCE_DIR}/src/lib/buffer.cpp)

target_include_directories(test_router PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_router PRIVATE cxx_std_20)

add_test(NAME test_router COMMAND test_router)


#test interpreter_stub

file(GLOB ASTNode_SRC ${CMAKE_SOURCE_DIR}/src/interpreter/ASTnodes/*.cpp)

add_executable(test_interpreter_stub test_interpreter_stub.cpp
                                    ${CMAKE_SOURCE_DIR}/tests/stubs/environment_stub.cpp
                                    ${CMAKE_SOURCE_DIR}/src/interpreter/interpreter.cpp
                                    ${CMAKE_SOURCE_DIR}/src/interpreter/func.cpp
                                    ${ASTNode_SRC})

target_include_directories(test_interpreter_stub PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_interpreter_stub PRIVATE cxx_std_20)

add_test(NAME test_interpreter_stub COMMAND test_interpreter_stub)

#test login_handler

add_executable(test_login test_login.cpp
                                    ${CMAKE_SOURCE_DIR}/src/utils/login_handler.cpp
                                    ${CMAKE_SOURCE_DIR}/src/lib/http_request.cpp
                                    ${CMAKE_SOURCE_DIR}/src/lib/http_response.cpp
                                    ${CMAKE_SOURCE_DIR}/src/lib/log.cpp
                                    ${CMAKE_SOURCE_DIR}/src/lib/buffer.cpp
                                    ${CMAKE_SOURCE_DIR}/tests/stubs/dbpool_stub.cpp
                                    ${CMAKE_SOURCE_DIR}/tests/stubs/environment_stub.cpp
                                    ${CMAKE_SOURCE_DIR}/tests/stubs/interpreter_stub.cpp
                                    ${CMAKE_SOURCE_DIR}/tests/stubs/session_stub.cpp)

target_link_libraries(test_login PRIVATE mysqlcppconn)

target_include_directories(test_login PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_login PRIVATE cxx_std_20)

add_test(NAME test_login COMMAND test_login)


#test chat_handler

add_executable(test_chat test_chat.cpp
                                    ${CMAKE_SOURCE_DIR}/src/utils/chat_handler.cpp
                                    ${CMAKE_SOURCE_DIR}/src/lib/http_request.cpp
                                    ${CMAKE_SOURCE_DIR}/src/lib/http_response.cpp
                                    ${CMAKE_SOURCE_DIR}/src/lib/log.cpp
                                    ${CMAKE_SOURCE_DIR}/src/lib/buffer.cpp
                                    ${CMAKE_SOURCE_DIR}/tests/stubs/environment_stub.cpp
                                    ${CMAKE_SOURCE_DIR}/tests/stubs/interpreter_stub.cpp
                                    ${CMAKE_SOURCE_DIR}/tests/stubs/session_stub.cpp)

target_include_directories(test_chat PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_features(test_chat PRIVATE cxx_std_20)

add_test(NAME test_chat COMMAND test_chat)
