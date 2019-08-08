project(room)
cmake_minimum_required(VERSION 3.0)

include_directories(
  server_actors
  protos
)

add_definitions(-std=c++11)

set(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -DBOOST_LOG_DYN_LINK")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -DBOOST_LOG_DYN_LINK")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

add_executable(
  word_count_server
  word_count_server.cc
  reflect/server_reflect.cc
  protos/word_count.pb.cc
)

target_link_libraries(word_count_server actor_server pthread protobuf boost_thread boost_log)

