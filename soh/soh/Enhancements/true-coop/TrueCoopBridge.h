#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void TrueCoop_SetDebugEnabled(bool enabled);
bool TrueCoop_IsDebugEnabled(void);

void TrueCoop_LogEnemyDamageEventC(
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
    bool died);

void TrueCoop_LogEnemyKillEventC(
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
    int16_t hpAtKill);

void TrueCoop_LogEnemyTransformEventC(
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
    float speedXZ);

void TrueCoop_LogEnemyStateEventC(
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
    uint16_t bgCheckFlags);

#ifdef __cplusplus
}
#endif
