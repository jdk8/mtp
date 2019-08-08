project(mtp)
cmake_minimum_required(VERSION 3.0)

include_directories(
  core/client
  core/server
  core/common
)

add_definitions(-std=c++11)

set(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -g -ggdb -DBOOST_LOG_DYN_LINK")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -Wall -DBOOST_LOG_DYN_LINK")

add_library(
  actor_client
  SHARED
  core/client/tcp_client.cc
  core/client/actor_client.cc
  core/client/client_actor.cc
  core/common/epoller.cc
  core/common/lt_epoller.cc
  core/common/et_epoller.cc
  core/common/myrpc_channel.cc
  core/common/rpc_package.pb.cc
  core/common/actor.pb.cc
  core/common/mail.cc
  core/common/timer.cc
)

add_library(
  actor_server
  SHARED
  core/server/messager.cc
  core/server/message_type.cc
  core/server/worker.cc
  core/server/tcp_server.cc
  core/server/actor_server.cc
  core/server/server_actor.cc
  core/common/epoller.cc
  core/common/lt_epoller.cc
  core/common/et_epoller.cc
  core/common/myrpc_channel.cc
  core/common/rpc_package.pb.cc
  core/common/actor.pb.cc
  core/common/mail.cc
  core/common/timer.cc
)

target_link_libraries(actor_client)
target_link_libraries(actor_server)

install(FILES core/client/client_actor.h DESTINATION include/mtp)
install(FILES core/client/tcp_client.h DESTINATION include/mtp)
install(FILES core/client/actor_client.h DESTINATION include/mtp)
install(FILES core/server/server_actor.h DESTINATION include/mtp)
install(FILES core/server/actor_server.h DESTINATION include/mtp)
install(FILES core/server/tcp_server.h DESTINATION include/mtp)
install(FILES core/server/messager.h DESTINATION include/mtp)
install(FILES core/server/worker.h DESTINATION include/mtp)
install(FILES core/server/message_type.h DESTINATION include/mtp)
install(FILES core/common/myrpc_channel.h DESTINATION include/mtp)
install(FILES core/common/myrpc_controller.h DESTINATION include/mtp)
install(FILES core/common/rpc_package.pb.h DESTINATION include/mtp)
install(FILES core/common/epoller.h DESTINATION include/mtp)
install(FILES core/common/lt_epoller.h DESTINATION include/mtp)
install(FILES core/common/et_epoller.h DESTINATION include/mtp)
install(FILES core/common/actor.h DESTINATION include/mtp)
install(FILES core/common/const.h DESTINATION include/mtp)
install(FILES core/common/epoller.h DESTINATION include/mtp)
install(FILES core/common/mail.h DESTINATION include/mtp)
install(FILES core/common/reflect.h DESTINATION include/mtp)
install(FILES core/common/timer.h DESTINATION include/mtp)
install(FILES core/common/logger.h DESTINATION include/mtp)
install(FILES core/common/actor.pb.h DESTINATION include/mtp)
install(FILES core/common/helper.h DESTINATION include/mtp)
install(FILES core/common/object_pool.h DESTINATION include/mtp)
install(TARGETS actor_client DESTINATION lib)
install(TARGETS actor_server DESTINATION lib)
