#include <mtp/actor_client.h>

int main(int argc, char* argv[]) {
  if (argc != 4) {
    LOG_ERROR << "Usage: room_client <host> <port> <actor_dir>\n";
    return 1;
  }
  mtp::ActorClient::Instance().Init(argv[1], std::stoi(argv[2]), argv[3]);
  mtp::ActorClient::Instance().RunForever();
  return 0;
}

