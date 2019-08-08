// Generated code

#include <mtp/reflect.h>
#include "word_count.h"
#include "word_count.pb.h"

namespace mtp {

Actor* NewActor(const std::string& actor_name) {
  if (actor_name == "WordCount")
    return new WordCount;
  return NULL;
}

void CallActorMethod(Actor* actor, const std::string& method, const std::string& message) {
  if (method == "RecvWordCountPairs") {
    WordCountPairs arg;
    arg.ParseFromString(message);
    static_cast<WordCount*>(actor)->RecvWordCountPairs(arg);
    return;
  }
}

void CallClientService(Actor* actor, const std::string& method, const Mail& mail) {
  if (method == "ClientJoin") {
    static_cast<ClientService*>(actor)->ClientJoin(mail);
    return;
  }
  if (method == "ClientLeave") {
    static_cast<ClientService*>(actor)->ClientLeave(mail);
    return;
  }
}

}