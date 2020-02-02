cmake_minimum_required(VERSION 3.0)

project(edison-ip-pager VERSION 1.0)

set(CMAKE_CXX_FLAGS "-static -w -Wall -Wextra -Wsign-conversion -g")

add_subdirectory(serial)
add_subdirectory(server)

add_executable(message_http server/message_http.cpp)

# Link libraries
target_link_libraries(message_http simple-web-server)
target_link_libraries(message_http serial-io)

# Link Boost
find_package(Boost 1.53.0 COMPONENTS system thread filesystem REQUIRED)
target_link_libraries(message_http ${Boost_LIBRARIES})
target_include_directories(message_http PRIVATE ${Boost_INCLUDE_DIR})

install(FILES server_http.hpp client_http.hpp crypto.hpp utility.hpp status_code.hpp DESTINATION include/simple-web-server)
