#include "TrueCoopBridge.h"

#include "soh/Enhancements/game-interactor/GameInteractor.h"
#include "soh/ShipInit.hpp"

extern "C" {
#include "variables.h"
#include "functions.h"
#include "macros.h"
extern PlayState* gPlayState;
}

namespace {

#define CVAR_TRUE_COOP_ENABLED CVAR_REMOTE("TrueCoop.Enabled")

bool TrueCoop_IsEnabled() {
    return CVarGetInteger(CVAR_TRUE_COOP_ENABLED, 0) != 0;
}

bool TrueCoop_IsSyncCandidateActor(Actor* actor) {
    if (actor == nullptr) {
        return false;
    }

    return actor->category == ACTORCAT_ENEMY || actor->category == ACTORCAT_BOSS;
}

void TrueCoop_LogActorKill(void* actorPtr) {
    Actor* actor = static_cast<Actor*>(actorPtr);

    TrueCoop_SetDebugEnabled(TrueCoop_IsEnabled());

    if (!TrueCoop_IsDebugEnabled() || !TrueCoop_IsSyncCandidateActor(actor)) {
        return;
    }

    const int16_t sceneId = gPlayState != nullptr ? gPlayState->sceneNum : -1;
    const int16_t roomId = actor->room;

    TrueCoop_LogEnemyKillEventC(
        sceneId,
        roomId,
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

void RegisterTrueCoopHooks() {
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnActorKill>(TrueCoop_LogActorKill);
}

} // namespace

static RegisterShipInitFunc initFunc(RegisterTrueCoopHooks, { CVAR_TRUE_COOP_ENABLED });
