#include "TrueCoopBridge.h"

#include "TrueCoop.h"

namespace {

TrueCoop::ActorIdentity BuildIdentity(
    uint32_t coOpId,
    int16_t sceneId,
    int16_t roomId,
    int16_t actorId,
    int16_t actorCategory,
    int16_t actorParams,
    int16_t actorListIndex,
    float positionX,
    float positionY,
    float positionZ,
    int16_t rotationX,
    int16_t rotationY,
    int16_t rotationZ) {
    TrueCoop::ActorIdentity identity;
    identity.coOpId = coOpId;
    identity.sceneId = sceneId;
    identity.roomId = roomId;
    identity.actorId = actorId;
    identity.actorCategory = actorCategory;
    identity.actorParams = actorParams;
    identity.actorListIndex = actorListIndex;
    identity.positionX = positionX;
    identity.positionY = positionY;
    identity.positionZ = positionZ;
    identity.rotationX = rotationX;
    identity.rotationY = rotationY;
    identity.rotationZ = rotationZ;
    return identity;
}

} // namespace

void TrueCoop_SetDebugEnabled(bool enabled) {
    TrueCoop::SetFeatureState(enabled ? TrueCoop::FeatureState::DebugOnly : TrueCoop::FeatureState::Disabled);
}

bool TrueCoop_IsDebugEnabled(void) {
    return TrueCoop::IsDebugEnabled();
}

void TrueCoop_LogEnemyDamageEventC(
    uint32_t coOpId,
    int16_t sceneId,
    int16_t roomId,
    int16_t actorId,
    int16_t actorCategory,
    int16_t actorParams,
    int16_t actorListIndex,
    float positionX,
    float positionY,
    float positionZ,
    int16_t rotationX,
    int16_t rotationY,
    int16_t rotationZ,
    int16_t hpBefore,
    int16_t damageAmount,
    int16_t hpAfter,
    bool died) {
    TrueCoop::EnemyDamageEvent event;
    event.enemy = BuildIdentity(coOpId, sceneId, roomId, actorId, actorCategory, actorParams, actorListIndex, positionX,
                                positionY, positionZ, rotationX, rotationY, rotationZ);
    event.hpBefore = hpBefore;
    event.damageAmount = damageAmount;
    event.hpAfter = hpAfter;
    event.died = died;

    TrueCoop::LogEnemyDamageEvent(event);
}

void TrueCoop_LogEnemyKillEventC(
    uint32_t coOpId,
    int16_t sceneId,
    int16_t roomId,
    int16_t actorId,
    int16_t actorCategory,
    int16_t actorParams,
    int16_t actorListIndex,
    float positionX,
    float positionY,
    float positionZ,
    int16_t rotationX,
    int16_t rotationY,
    int16_t rotationZ,
    int16_t hpAtKill) {
    TrueCoop::EnemyKillEvent event;
    event.enemy = BuildIdentity(coOpId, sceneId, roomId, actorId, actorCategory, actorParams, actorListIndex, positionX,
                                positionY, positionZ, rotationX, rotationY, rotationZ);
    event.hpAtKill = hpAtKill;

    TrueCoop::LogEnemyKillEvent(event);
}

void TrueCoop_LogEnemyTransformEventC(
    uint32_t coOpId,
    int16_t sceneId,
    int16_t roomId,
    int16_t actorId,
    int16_t actorCategory,
    int16_t actorParams,
    int16_t actorListIndex,
    float positionX,
    float positionY,
    float positionZ,
    int16_t rotationX,
    int16_t rotationY,
    int16_t rotationZ,
    float velocityX,
    float velocityY,
    float velocityZ,
    float speedXZ) {
    TrueCoop::EnemyTransformEvent event;
    event.enemy = BuildIdentity(coOpId, sceneId, roomId, actorId, actorCategory, actorParams, actorListIndex, positionX,
                                positionY, positionZ, rotationX, rotationY, rotationZ);
    event.velocityX = velocityX;
    event.velocityY = velocityY;
    event.velocityZ = velocityZ;
    event.speedXZ = speedXZ;

    TrueCoop::LogEnemyTransformEvent(event);
}

void TrueCoop_LogEnemyStateEventC(
    uint32_t coOpId,
    int16_t sceneId,
    int16_t roomId,
    int16_t actorId,
    int16_t actorCategory,
    int16_t actorParams,
    int16_t actorListIndex,
    float positionX,
    float positionY,
    float positionZ,
    int16_t rotationX,
    int16_t rotationY,
    int16_t rotationZ,
    int16_t health,
    int16_t params,
    int16_t freezeTimer,
    int16_t colorFilterTimer,
    uint16_t bgCheckFlags) {
    TrueCoop::EnemyStateEvent event;
    event.enemy = BuildIdentity(coOpId, sceneId, roomId, actorId, actorCategory, actorParams, actorListIndex, positionX,
                                positionY, positionZ, rotationX, rotationY, rotationZ);
    event.health = health;
    event.params = params;
    event.freezeTimer = freezeTimer;
    event.colorFilterTimer = colorFilterTimer;
    event.bgCheckFlags = bgCheckFlags;

    TrueCoop::LogEnemyStateEvent(event);
}
