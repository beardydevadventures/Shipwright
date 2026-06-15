#pragma once

#include <cstdint>
#include <string>

namespace TrueCoop {

enum class FeatureState : uint8_t {
    Disabled = 0,
    DebugOnly = 1,
};

struct ActorIdentity {
    int16_t sceneId = -1;
    int16_t roomId = -1;
    int16_t actorId = -1;
    int16_t actorCategory = -1;
    float positionX = 0.0f;
    float positionY = 0.0f;
    float positionZ = 0.0f;
    int16_t rotationX = 0;
    int16_t rotationY = 0;
    int16_t rotationZ = 0;
};

struct EnemyDamageEvent {
    ActorIdentity enemy;
    int16_t hpBefore = -1;
    int16_t damageAmount = 0;
    int16_t hpAfter = -1;
    bool died = false;
};

FeatureState GetFeatureState();
bool IsDebugEnabled();
void SetFeatureState(FeatureState state);

std::string DescribeActorIdentity(const ActorIdentity& identity);
std::string DescribeEnemyDamageEvent(const EnemyDamageEvent& event);
void LogEnemyDamageEvent(const EnemyDamageEvent& event);

} // namespace TrueCoop
