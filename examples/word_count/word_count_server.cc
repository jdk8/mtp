#include <mtp/actor_server.h>

int main(int argc, char* argv[]) {
  if (argc != 4) {
    LOG_ERROR << "Usage: word_count_server <ip> <port> <actor_dir>";
    return 1;
  }
  mtp::ActorServer::Instance().Init(argv[1], std::atoi(argv[2]), argv[3]);
  mtp::ActorServer::Instance().RunForever();
  return 0;
}

