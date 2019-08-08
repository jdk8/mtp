#ifndef SERVER_REFLECT_H__
#define SERVER_REFLECT_H__

#include <iostream>
#include "server_actor.h"

namespace mtp {

Actor* NewActor(const std::string& actor_name);
void CallActorMethod(Actor* actor, const std::string& method, const std::string& message);
void CallClientService(Actor* actor, const std::string& method, const Mail& mail);

}

#endif