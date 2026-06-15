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
           << " params=" << identity.actorParams
           << " listIndex=" << identity.actorListIndex
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

std::string DescribeEnemyKillEvent(const EnemyKillEvent& event) {
    std::ostringstream stream;
    stream << "enemy{" << DescribeActorIdentity(event.enemy) << "}"
           << " hpAtKill=" << event.hpAtKill;
    return stream.str();
}

std::string DescribeEnemyTransformEvent(const EnemyTransformEvent& event) {
    std::ostringstream stream;
    stream << "enemy{" << DescribeActorIdentity(event.enemy) << "}"
           << " velocity=(" << event.velocityX << ", " << event.velocityY << ", " << event.velocityZ << ")"
           << " speedXZ=" << event.speedXZ;
    return stream.str();
}

std::string DescribeEnemyStateEvent(const EnemyStateEvent& event) {
    std::ostringstream stream;
    stream << "enemy{" << DescribeActorIdentity(event.enemy) << "}"
           << " health=" << event.health
           << " params=" << event.params
           << " freezeTimer=" << event.freezeTimer
           << " colorFilterTimer=" << event.colorFilterTimer
           << " bgCheckFlags=" << event.bgCheckFlags;
    return stream.str();
}

void LogEnemyDamageEvent(const EnemyDamageEvent& event) {
    if (!IsDebugEnabled()) {
        return;
    }

    std::printf("[TrueCoop] EnemyDamageEvent %s\n", DescribeEnemyDamageEvent(event).c_str());
}

void LogEnemyKillEvent(const EnemyKillEvent& event) {
    if (!IsDebugEnabled()) {
        return;
    }

    std::printf("[TrueCoop] EnemyKillEvent %s\n", DescribeEnemyKillEvent(event).c_str());
}

void LogEnemyTransformEvent(const EnemyTransformEvent& event) {
    if (!IsDebugEnabled()) {
        return;
    }

    std::printf("[TrueCoop] EnemyTransformEvent %s\n", DescribeEnemyTransformEvent(event).c_str());
}

void LogEnemyStateEvent(const EnemyStateEvent& event) {
    if (!IsDebugEnabled()) {
        return;
    }

    std::printf("[TrueCoop] EnemyStateEvent %s\n", DescribeEnemyStateEvent(event).c_str());
}

} // namespace TrueCoop
