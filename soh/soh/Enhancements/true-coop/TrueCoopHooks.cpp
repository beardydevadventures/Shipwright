#include "TrueCoopBridge.h"

#include "soh/Enhancements/game-interactor/GameInteractor.h"
#include "soh/ShipInit.hpp"

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
    int16_t health = -1;
};

std::unordered_map<uintptr_t, TrueCoopTrackedEnemyState> sTrackedEnemyStates;

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

void TrueCoop_LogActorDamage(Actor* actor, int16_t hpBefore, int16_t hpAfter) {
    const int16_t damageAmount = hpBefore - hpAfter;

    if (damageAmount <= 0) {
        return;
    }

    TrueCoop_LogEnemyDamageEventC(
        TrueCoop_GetSceneId(),
        actor->room,
        actor->id,
        actor->category,
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

void TrueCoop_TrackActorHealth(void* actorPtr) {
    Actor* actor = static_cast<Actor*>(actorPtr);

    TrueCoop_SetDebugEnabled(TrueCoop_IsEnabled());

    if (!TrueCoop_IsDebugEnabled() || !TrueCoop_IsSyncCandidateActor(actor)) {
        return;
    }

    const uintptr_t actorKey = reinterpret_cast<uintptr_t>(actor);
    const int16_t currentHealth = actor->colChkInfo.health;
    auto existingState = sTrackedEnemyStates.find(actorKey);

    if (existingState == sTrackedEnemyStates.end()) {
        sTrackedEnemyStates[actorKey] = TrueCoopTrackedEnemyState{ currentHealth };
        return;
    }

    const int16_t previousHealth = existingState->second.health;

    if (currentHealth < previousHealth) {
        TrueCoop_LogActorDamage(actor, previousHealth, currentHealth);
    }

    existingState->second.health = currentHealth;
}

void TrueCoop_LogActorKill(void* actorPtr) {
    Actor* actor = static_cast<Actor*>(actorPtr);

    TrueCoop_SetDebugEnabled(TrueCoop_IsEnabled());

    if (!TrueCoop_IsDebugEnabled() || !TrueCoop_IsSyncCandidateActor(actor)) {
        return;
    }

    TrueCoop_LogEnemyKillEventC(
        TrueCoop_GetSceneId(),
        actor->room,
        actor->id,
        actor->category,
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
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnActorUpdate>(TrueCoop_TrackActorHealth);
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnActorKill>(TrueCoop_LogActorKill);
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnActorDestroy>(TrueCoop_UntrackActor);
}

} // namespace

static RegisterShipInitFunc initFunc(RegisterTrueCoopHooks, { CVAR_TRUE_COOP_ENABLED });
