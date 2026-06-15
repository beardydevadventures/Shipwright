#include "TrueCoopBridge.h"

#include "soh/Enhancements/game-interactor/GameInteractor.h"
#include "soh/ObjectExtension/ActorListIndex.h"
#include "soh/ShipInit.hpp"

#include <cmath>
#include <cstdint>
#include <unordered_map>

extern "C" {
#include "variables.h"
#include "functions.h"
#include "macros.h"
extern PlayState* gPlayState;
}

namespace {

#define CVAR_TRUE_COOP_ENABLED CVAR_REMOTE("TrueCoop.Enabled")

struct TrueCoopTrackedEnemyState {
    uint32_t coOpId = 0;
    int16_t health = -1;
    int16_t params = 0;
    int16_t freezeTimer = 0;
    int16_t colorFilterTimer = 0;
    uint16_t bgCheckFlags = 0;
    float positionX = 0.0f;
    float positionY = 0.0f;
    float positionZ = 0.0f;
    int16_t rotationX = 0;
    int16_t rotationY = 0;
    int16_t rotationZ = 0;
    uint32_t lastTransformFrame = 0;
};

std::unordered_map<uintptr_t, TrueCoopTrackedEnemyState> sTrackedEnemyStates;
uint32_t sNextCoOpEnemyId = 1;

bool TrueCoop_IsEnabled() {
    return CVarGetInteger(CVAR_TRUE_COOP_ENABLED, 0) != 0;
}

bool TrueCoop_IsSyncCandidateActor(Actor* actor) {
    if (actor == nullptr) {
        return false;
    }

    return actor->category == ACTORCAT_ENEMY || actor->category == ACTORCAT_BOSS;
}

int16_t TrueCoop_GetSceneId() {
    return gPlayState != nullptr ? gPlayState->sceneNum : -1;
}

uint32_t TrueCoop_GetGameplayFrame() {
    return gPlayState != nullptr ? gPlayState->gameplayFrames : 0;
}

int16_t TrueCoop_GetActorListIndex(Actor* actor) {
    return actor != nullptr ? GetActorListIndex(actor) : -1;
}

uint32_t TrueCoop_GetOrAssignCoOpId(Actor* actor) {
    if (actor == nullptr) {
        return 0;
    }

    const uintptr_t actorKey = reinterpret_cast<uintptr_t>(actor);
    auto existingState = sTrackedEnemyStates.find(actorKey);

    if (existingState != sTrackedEnemyStates.end()) {
        return existingState->second.coOpId;
    }

    TrueCoopTrackedEnemyState state;
    state.coOpId = sNextCoOpEnemyId++;
    sTrackedEnemyStates[actorKey] = state;
    return state.coOpId;
}

bool TrueCoop_HasTransformMovedEnough(Actor* actor, const TrueCoopTrackedEnemyState& state) {
    const float dx = actor->world.pos.x - state.positionX;
    const float dy = actor->world.pos.y - state.positionY;
    const float dz = actor->world.pos.z - state.positionZ;
    const float distanceSq = (dx * dx) + (dy * dy) + (dz * dz);
    const bool positionChanged = distanceSq >= 25.0f;
    const bool rotationChanged = actor->world.rot.x != state.rotationX || actor->world.rot.y != state.rotationY ||
                                 actor->world.rot.z != state.rotationZ;

    return positionChanged || rotationChanged;
}

void TrueCoop_LogActorDamage(Actor* actor, int16_t hpBefore, int16_t hpAfter) {
    const int16_t damageAmount = hpBefore - hpAfter;

    if (damageAmount <= 0) {
        return;
    }

    TrueCoop_LogEnemyDamageEventC(
        TrueCoop_GetOrAssignCoOpId(actor),
        TrueCoop_GetSceneId(),
        actor->room,
        actor->id,
        actor->category,
        actor->params,
        TrueCoop_GetActorListIndex(actor),
        actor->world.pos.x,
        actor->world.pos.y,
        actor->world.pos.z,
        actor->world.rot.x,
        actor->world.rot.y,
        actor->world.rot.z,
        hpBefore,
        damageAmount,
        hpAfter,
        hpAfter <= 0);
}

void TrueCoop_LogActorTransform(Actor* actor) {
    TrueCoop_LogEnemyTransformEventC(
        TrueCoop_GetOrAssignCoOpId(actor),
        TrueCoop_GetSceneId(),
        actor->room,
        actor->id,
        actor->category,
        actor->params,
        TrueCoop_GetActorListIndex(actor),
        actor->world.pos.x,
        actor->world.pos.y,
        actor->world.pos.z,
        actor->world.rot.x,
        actor->world.rot.y,
        actor->world.rot.z,
        actor->velocity.x,
        actor->velocity.y,
        actor->velocity.z,
        actor->speedXZ);
}

void TrueCoop_LogActorState(Actor* actor) {
    TrueCoop_LogEnemyStateEventC(
        TrueCoop_GetOrAssignCoOpId(actor),
        TrueCoop_GetSceneId(),
        actor->room,
        actor->id,
        actor->category,
        actor->params,
        TrueCoop_GetActorListIndex(actor),
        actor->world.pos.x,
        actor->world.pos.y,
        actor->world.pos.z,
        actor->world.rot.x,
        actor->world.rot.y,
        actor->world.rot.z,
        actor->colChkInfo.health,
        actor->params,
        actor->freezeTimer,
        actor->colorFilterTimer,
        actor->bgCheckFlags);
}

TrueCoopTrackedEnemyState TrueCoop_BuildTrackedState(Actor* actor, uint32_t coOpId) {
    TrueCoopTrackedEnemyState state;
    state.coOpId = coOpId;
    state.health = actor->colChkInfo.health;
    state.params = actor->params;
    state.freezeTimer = actor->freezeTimer;
    state.colorFilterTimer = actor->colorFilterTimer;
    state.bgCheckFlags = actor->bgCheckFlags;
    state.positionX = actor->world.pos.x;
    state.positionY = actor->world.pos.y;
    state.positionZ = actor->world.pos.z;
    state.rotationX = actor->world.rot.x;
    state.rotationY = actor->world.rot.y;
    state.rotationZ = actor->world.rot.z;
    state.lastTransformFrame = TrueCoop_GetGameplayFrame();
    return state;
}

void TrueCoop_AssignActorId(void* actorPtr) {
    Actor* actor = static_cast<Actor*>(actorPtr);

    if (!TrueCoop_IsSyncCandidateActor(actor)) {
        return;
    }

    const uint32_t coOpId = TrueCoop_GetOrAssignCoOpId(actor);
    const uintptr_t actorKey = reinterpret_cast<uintptr_t>(actor);
    sTrackedEnemyStates[actorKey] = TrueCoop_BuildTrackedState(actor, coOpId);
}

void TrueCoop_TrackActorState(void* actorPtr) {
    Actor* actor = static_cast<Actor*>(actorPtr);

    TrueCoop_SetDebugEnabled(TrueCoop_IsEnabled());

    if (!TrueCoop_IsDebugEnabled() || !TrueCoop_IsSyncCandidateActor(actor)) {
        return;
    }

    const uintptr_t actorKey = reinterpret_cast<uintptr_t>(actor);
    const uint32_t coOpId = TrueCoop_GetOrAssignCoOpId(actor);
    auto existingState = sTrackedEnemyStates.find(actorKey);

    if (existingState == sTrackedEnemyStates.end()) {
        sTrackedEnemyStates[actorKey] = TrueCoop_BuildTrackedState(actor, coOpId);
        TrueCoop_LogActorState(actor);
        TrueCoop_LogActorTransform(actor);
        return;
    }

    TrueCoopTrackedEnemyState& previous = existingState->second;

    if (actor->colChkInfo.health < previous.health) {
        TrueCoop_LogActorDamage(actor, previous.health, actor->colChkInfo.health);
    }

    if (actor->params != previous.params || actor->freezeTimer != previous.freezeTimer ||
        actor->colorFilterTimer != previous.colorFilterTimer || actor->bgCheckFlags != previous.bgCheckFlags) {
        TrueCoop_LogActorState(actor);
    }

    const uint32_t currentFrame = TrueCoop_GetGameplayFrame();
    const bool transformIntervalElapsed = (currentFrame - previous.lastTransformFrame) >= 20;

    if (transformIntervalElapsed && TrueCoop_HasTransformMovedEnough(actor, previous)) {
        TrueCoop_LogActorTransform(actor);
        previous.lastTransformFrame = currentFrame;
    }

    previous = TrueCoop_BuildTrackedState(actor, coOpId);
}

void TrueCoop_LogActorKill(void* actorPtr) {
    Actor* actor = static_cast<Actor*>(actorPtr);

    TrueCoop_SetDebugEnabled(TrueCoop_IsEnabled());

    if (!TrueCoop_IsDebugEnabled() || !TrueCoop_IsSyncCandidateActor(actor)) {
        return;
    }

    TrueCoop_LogEnemyKillEventC(
        TrueCoop_GetOrAssignCoOpId(actor),
        TrueCoop_GetSceneId(),
        actor->room,
        actor->id,
        actor->category,
        actor->params,
        TrueCoop_GetActorListIndex(actor),
        actor->world.pos.x,
        actor->world.pos.y,
        actor->world.pos.z,
        actor->world.rot.x,
        actor->world.rot.y,
        actor->world.rot.z,
        actor->colChkInfo.health);
}

void TrueCoop_UntrackActor(void* actorPtr) {
    Actor* actor = static_cast<Actor*>(actorPtr);

    if (!TrueCoop_IsSyncCandidateActor(actor)) {
        return;
    }

    sTrackedEnemyStates.erase(reinterpret_cast<uintptr_t>(actor));
}

void RegisterTrueCoopHooks() {
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnActorSpawn>(TrueCoop_AssignActorId);
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnActorUpdate>(TrueCoop_TrackActorState);
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnActorKill>(TrueCoop_LogActorKill);
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnActorDestroy>(TrueCoop_UntrackActor);
}

} // namespace

static RegisterShipInitFunc initFunc(RegisterTrueCoopHooks, { CVAR_TRUE_COOP_ENABLED });
