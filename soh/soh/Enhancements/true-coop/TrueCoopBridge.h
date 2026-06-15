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
    float positionX,
    float positionY,
    float positionZ,
    int16_t rotationX,
    int16_t rotationY,
    int16_t rotationZ,
    int16_t hpAtKill);

#ifdef __cplusplus
}
#endif
