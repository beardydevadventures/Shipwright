#include "TrueCoopBridge.h"

#include "TrueCoop.h"

void TrueCoop_SetDebugEnabled(bool enabled) {
    TrueCoop::SetFeatureState(enabled ? TrueCoop::FeatureState::DebugOnly : TrueCoop::FeatureState::Disabled);
}

bool TrueCoop_IsDebugEnabled(void) {
    return TrueCoop::IsDebugEnabled();
}

void TrueCoop_LogEnemyDamageEventC(
    int16_t sceneId,
    int16_t roomId,
    int16_t actorId,
    int16_t actorCategory,
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
    event.enemy.sceneId = sceneId;
    event.enemy.roomId = roomId;
    event.enemy.actorId = actorId;
    event.enemy.actorCategory = actorCategory;
    event.enemy.positionX = positionX;
    event.enemy.positionY = positionY;
    event.enemy.positionZ = positionZ;
    event.enemy.rotationX = rotationX;
    event.enemy.rotationY = rotationY;
    event.enemy.rotationZ = rotationZ;
    event.hpBefore = hpBefore;
    event.damageAmount = damageAmount;
    event.hpAfter = hpAfter;
    event.died = died;

    TrueCoop::LogEnemyDamageEvent(event);
}
