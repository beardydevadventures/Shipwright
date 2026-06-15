# True Co-op Prototype Plan

This branch is for exploring true co-op behaviour in Ship of Harkinian without changing normal single-player gameplay.

## Current scope

The first target is enemy health/damage sync, not full multiplayer.

The first milestone is:

> When Link damages a normal enemy, produce a clean debug event that identifies the enemy and records the damage/HP result.

Enemy kill sync is also included as a safety net:

> When an enemy dies, produce a host-authoritative kill event so every client can force-remove the same enemy even if local HP or AI state has drifted.

Networking, remote players, save sync, boss sync, cutscene sync, dungeon state sync, and item drop sync are intentionally out of scope for the first pass.

## Added scaffold

The initial scaffold lives in:

```text
soh/soh/Enhancements/true-coop/
```

Files:

- `TrueCoop.h`
- `TrueCoop.cpp`
- `TrueCoopBridge.h`
- `TrueCoopBridge.cpp`
- `TrueCoopHooks.cpp`

The scaffold defines:

- `FeatureState`
- `ActorIdentity`
- `EnemyDamageEvent`
- `EnemyKillEvent`
- debug enable/disable helpers
- debug formatting helpers
- `LogEnemyDamageEvent`
- `LogEnemyKillEvent`

The feature is disabled by default and is controlled by:

```text
Remote.TrueCoop.Enabled
```

The toggle is available under:

```text
Network > True Co-op > Enable True Co-op Events
```

## Enemy damage event shape

The current damage observer tracks enemy/boss HP through `GameInteractor::OnActorUpdate`.

It stores the previous `actor->colChkInfo.health` per actor pointer. When the current health is lower than the previous health, it emits an `EnemyDamageEvent`.

This catches normal `Actor_ApplyDamage` results and also catches direct/manual HP changes such as enemies that set health to zero without calling `Actor_ApplyDamage`.

The event contains:

- scene id
- room id
- actor id/type
- actor category
- actor position
- actor rotation
- HP before damage
- damage amount
- HP after damage
- died flag

`Actor_ApplyDamage` itself is very small:

```c
u8 Actor_ApplyDamage(Actor* actor) {
    if (actor->colChkInfo.damage >= actor->colChkInfo.health) {
        actor->colChkInfo.health = 0;
    } else {
        actor->colChkInfo.health -= actor->colChkInfo.damage;
    }

    return actor->colChkInfo.health;
}
```

The observer approach avoids editing core actor code while still capturing the resulting HP changes.

## Enemy kill event shape

The current kill event contains:

- scene id
- room id
- actor id/type
- actor category
- actor position
- actor rotation
- HP at kill

The kill hook uses `GameInteractor::OnActorKill` and filters to enemy/boss actor categories.

## Enemy state sync direction

The intended multiplayer model is host-authoritative for enemies.

Recommended order:

1. Sync enemy HP/damage events.
2. Sync enemy kill/removal events as a hard correction path.
3. Sync enemy position/rotation from the host.
4. Sync only the minimum AI/action state needed to keep animations and behaviour coherent.
5. Avoid fully simulating the same enemy AI independently on every client.

The host should own enemy AI and enemy state. Clients should mostly send player action/damage requests and receive authoritative enemy HP, kill, position, rotation, and current action state.

This should avoid enemies fighting their own alternate timelines across clients.

## MVP path

1. Add the opt-in menu toggle.
2. Hook `Actor_Kill` through `GameInteractor::OnActorKill` for death events.
3. Track enemy HP changes through `GameInteractor::OnActorUpdate`.
4. Emit `EnemyDamageEvent` when enemy HP drops.
5. Log events only when true-coop debug mode is enabled.
6. Add a fake local receive/apply path for testing authoritative state updates without networking.
7. Add a network message later.
8. Let host/server own enemy HP.
9. Broadcast authoritative HP/death state to clients.
10. Add host-owned position/rotation sync.
11. Add minimal host-owned AI/action sync only if position/rotation alone is not enough.

## Rules for this branch

- Keep normal single-player behaviour unchanged by default.
- Keep the feature behind explicit opt-in/debug state until proven stable.
- Health sync remains the priority; kill sync is the drift-correction safety net.
- Host should own enemy AI/state long term.
- Avoid boss, dungeon, cutscene, save-file, and item-drop sync until normal enemy damage is reliable.
- Prefer small reviewable commits.

## Known unknowns

- Best stable identity for actor instances across clients.
- Whether room-local actor indexes are stable enough or need a generated co-op id.
- Which enemy action/AI fields need to be synced after position/rotation.
- How much of Anchor should be reused versus kept separate.
