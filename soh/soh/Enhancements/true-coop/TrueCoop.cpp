#include "TrueCoop.h"

#include <cstdio>
#include <sstream>

namespace TrueCoop {
namespace {
FeatureState gFeatureState = FeatureState::Disabled;
}

FeatureState GetFeatureState() {
    return gFeatureState;
}

bool IsDebugEnabled() {
    return gFeatureState == FeatureState::DebugOnly;
}

void SetFeatureState(FeatureState state) {
    gFeatureState = state;
}

std::string DescribeActorIdentity(const ActorIdentity& identity) {
    std::ostringstream stream;
    stream << "scene=" << identity.sceneId
           << " room=" << identity.roomId
           << " actor=" << identity.actorId
           << " category=" << identity.actorCategory
           << " pos=(" << identity.positionX << ", " << identity.positionY << ", " << identity.positionZ << ")"
           << " rot=(" << identity.rotationX << ", " << identity.rotationY << ", " << identity.rotationZ << ")";
    return stream.str();
}

std::string DescribeEnemyDamageEvent(const EnemyDamageEvent& event) {
    std::ostringstream stream;
    stream << "enemy{" << DescribeActorIdentity(event.enemy) << "}"
           << " hpBefore=" << event.hpBefore
           << " damage=" << event.damageAmount
           << " hpAfter=" << event.hpAfter
           << " died=" << (event.died ? "true" : "false");
    return stream.str();
}

void LogEnemyDamageEvent(const EnemyDamageEvent& event) {
    if (!IsDebugEnabled()) {
        return;
    }

    const std::string description = DescribeEnemyDamageEvent(event);
    std::printf("[TrueCoop] EnemyDamageEvent %s\n", description.c_str());

    // TODO(true-coop): Send this damage event to a host/server once a network layer exists.
    // TODO(true-coop): Let the host validate the actor identity and damage amount.
    // TODO(true-coop): Broadcast the authoritative enemy HP/death state back to every client.
}

} // namespace TrueCoop
