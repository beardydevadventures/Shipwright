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
    int16_t actorParams = 0;
    int16_t actorListIndex = -1;
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

struct EnemyKillEvent {
    ActorIdentity enemy;
    int16_t hpAtKill = -1;
};

struct EnemyTransformEvent {
    ActorIdentity enemy;
    float velocityX = 0.0f;
    float velocityY = 0.0f;
    float velocityZ = 0.0f;
    float speedXZ = 0.0f;
};

struct EnemyStateEvent {
    ActorIdentity enemy;
    int16_t health = -1;
    int16_t params = 0;
    int16_t freezeTimer = 0;
    int16_t colorFilterTimer = 0;
    uint16_t bgCheckFlags = 0;
};

FeatureState GetFeatureState();
bool IsDebugEnabled();
void SetFeatureState(FeatureState state);

std::string DescribeActorIdentity(const ActorIdentity& identity);
std::string DescribeEnemyDamageEvent(const EnemyDamageEvent& event);
std::string DescribeEnemyKillEvent(const EnemyKillEvent& event);
std::string DescribeEnemyTransformEvent(const EnemyTransformEvent& event);
std::string DescribeEnemyStateEvent(const EnemyStateEvent& event);
void LogEnemyDamageEvent(const EnemyDamageEvent& event);
void LogEnemyKillEvent(const EnemyKillEvent& event);
void LogEnemyTransformEvent(const EnemyTransformEvent& event);
void LogEnemyStateEvent(const EnemyStateEvent& event);

} // namespace TrueCoop
