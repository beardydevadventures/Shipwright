# True Co-op Prototype Plan

This branch is for exploring true co-op behaviour in Ship of Harkinian without changing normal single-player gameplay.

## Current scope

The first target is enemy damage sync, not full multiplayer.

The first milestone is:

> When Link damages a normal enemy, produce a clean debug event that identifies the enemy and records the damage/HP result.

Networking, remote players, save sync, boss sync, cutscene sync, dungeon state sync, and item drop sync are intentionally out of scope for the first pass.

## Added scaffold

The initial scaffold lives in:

```text
soh/soh/Enhancements/true-coop/
```

Files:

- `TrueCoop.h`
- `TrueCoop.cpp`

The scaffold defines:

- `FeatureState`
- `ActorIdentity`
- `EnemyDamageEvent`
- debug enable/disable helpers
- debug formatting helpers
- `LogEnemyDamageEvent`

The feature is disabled by default and does not currently alter gameplay.

## Enemy damage event shape

The planned event contains:

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

This is intentionally simple so the next step can focus on finding the safest hook in the actor/damage pipeline.

## MVP path

1. Identify the safest damage application hook.
2. Build an `EnemyDamageEvent` when a normal enemy is damaged.
3. Log the event only when true-coop debug mode is enabled.
4. Add a fake local receive/apply path for testing authoritative state updates without networking.
5. Add a network message later.
6. Let host/server own enemy HP.
7. Broadcast authoritative HP/death state to clients.

## Rules for this branch

- Keep normal single-player behaviour unchanged by default.
- Keep the feature behind explicit opt-in/debug state until proven stable.
- Avoid boss, dungeon, cutscene, save-file, and item-drop sync until normal enemy damage is reliable.
- Prefer small reviewable commits.

## Known unknowns

- Exact location of the safest enemy damage hook.
- Whether each enemy actor exposes HP in a consistent way.
- Best stable identity for actor instances across clients.
- Whether room-local actor indexes are stable enough or need a generated co-op id.
- How much of Anchor should be reused versus kept separate.
