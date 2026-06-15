#include "TrueCoop.h"

#include <cstdio>
#include <sstream>
#include <unordered_map>

namespace TrueCoop {
namespace {
FeatureState gFeatureState = FeatureState::Disabled;
uint32_t gNextLocalId = 1;

struct EnemyIdRecord {
    uint32_t localId = 0;
    uint32_t sharedId = 0;
    EnemyIdentityDescriptor descriptor;
};

std::unordered_map<uint64_t, EnemyIdRecord> gEnemyIdRecords;
}

FeatureState GetFeatureState() {
    return gFeatureState;
}

bool IsDebugEnabled() {
    return gFeatureState == FeatureState::DebugOnly;
}

void SetFeatureState(FeatureState state) {
    gFeatureState = state;
}

uint32_t AssignLocalId(uint64_t actorKey, const EnemyIdentityDescriptor& descriptor) {
    auto it = gEnemyIdRecords.find(actorKey);
    if (it != gEnemyIdRecords.end()) {
        return it->second.localId;
    }

    EnemyIdRecord record;
    record.localId = gNextLocalId++;
    record.descriptor = descriptor;
    gEnemyIdRecords[actorKey] = record;
    return record.localId;
}

uint32_t GetLocalId(uint64_t actorKey) {
    auto it = gEnemyIdRecords.find(actorKey);
    return it != gEnemyIdRecords.end() ? it->second.localId : 0;
}

uint32_t GetSharedId(uint64_t actorKey) {
    auto it = gEnemyIdRecords.find(actorKey);
    if (it == gEnemyIdRecords.end()) {
        return 0;
    }

    return it->second.sharedId != 0 ? it->second.sharedId : it->second.localId;
}

uint32_t SetSharedId(uint64_t actorKey, uint32_t sharedId) {
    auto it = gEnemyIdRecords.find(actorKey);
    if (it == gEnemyIdRecords.end()) {
        return 0;
    }

    it->second.sharedId = sharedId;
    return sharedId;
}

bool HasSharedId(uint64_t actorKey) {
    auto it = gEnemyIdRecords.find(actorKey);
    return it != gEnemyIdRecords.end() && it->second.sharedId != 0;
}

void ReleaseId(uint64_t actorKey) {
    gEnemyIdRecords.erase(actorKey);
}

std::string DescribeActorIdentity(const ActorIdentity& identity) {
    std::ostringstream stream;
    stream << "coOpId=" << identity.coOpId
           << " scene=" << identity.sceneId
           << " room=" << identity.roomId
           << " actor=" << identity.actorId
           << " category=" << identity.actorCategory
           << " params=" << identity.actorParams
           << " listIndex=" << identity.actorListIndex
           << " pos=(" << identity.positionX << ", " << identity.positionY << ", " << identity.positionZ << ")"
           << " rot=(" << identity.rotationX << ", " << identity.rotationY << ", " << identity.rotationZ << ")";
    return stream.str();
}

std::string DescribeEnemyIdentityDescriptor(const EnemyIdentityDescriptor& descriptor) {
    std::ostringstream stream;
    stream << "scene=" << descriptor.sceneId
           << " room=" << descriptor.roomId
           << " actor=" << descriptor.actorId
           << " category=" << descriptor.actorCategory
           << " params=" << descriptor.actorParams
           << " listIndex=" << descriptor.actorListIndex
           << " spawnPos=(" << descriptor.spawnPositionX << ", " << descriptor.spawnPositionY << ", "
           << descriptor.spawnPositionZ << ")";
    return stream.str();
}

std::string DescribeEnemyDamageEvent(const EnemyDamageEvent& event) {
    std::ostringstream stream;
    stream << "enemy{" << DescribeActorIdentity(event.enemy) << "}"
           << " hpBefore=" << event.hpBefore
           << " damage=" << event.damageAmount
           << " hpAfter=" << event.hpAfter
           << " died=" << (event.died ? "true" : "false");
    return stream.str();
}

std::string DescribeEnemyKillEvent(const EnemyKillEvent& event) {
    std::ostringstream stream;
    stream << "enemy{" << DescribeActorIdentity(event.enemy) << "}"
           << " hpAtKill=" << event.hpAtKill;
    return stream.str();
}

std::string DescribeEnemyTransformEvent(const EnemyTransformEvent& event) {
    std::ostringstream stream;
    stream << "enemy{" << DescribeActorIdentity(event.enemy) << "}"
           << " velocity=(" << event.velocityX << ", " << event.velocityY << ", " << event.velocityZ << ")"
           << " speedXZ=" << event.speedXZ;
    return stream.str();
}

std::string DescribeEnemyStateEvent(const EnemyStateEvent& event) {
    std::ostringstream stream;
    stream << "enemy{" << DescribeActorIdentity(event.enemy) << "}"
           << " health=" << event.health
           << " params=" << event.params
           << " freezeTimer=" << event.freezeTimer
           << " colorFilterTimer=" << event.colorFilterTimer
           << " bgCheckFlags=" << event.bgCheckFlags;
    return stream.str();
}

void LogEnemyIdentityRequest(uint64_t actorKey, uint32_t localId, const EnemyIdentityDescriptor& descriptor) {
    if (!IsDebugEnabled()) {
        return;
    }

    std::printf("[TrueCoop] EnemyIdentityRequest localId=%u %s\n", localId,
                DescribeEnemyIdentityDescriptor(descriptor).c_str());
}

void LogEnemyIdentityMapping(uint64_t actorKey, uint32_t localId, uint32_t sharedId,
                             const EnemyIdentityDescriptor& descriptor) {
    if (!IsDebugEnabled()) {
        return;
    }

    std::printf("[TrueCoop] EnemyIdentityMapping localId=%u sharedId=%u %s\n", localId, sharedId,
                DescribeEnemyIdentityDescriptor(descriptor).c_str());
}

void LogEnemyDamageEvent(const EnemyDamageEvent& event) {
    if (!IsDebugEnabled()) {
        return;
    }

    std::printf("[TrueCoop] EnemyDamageEvent %s\n", DescribeEnemyDamageEvent(event).c_str());
}

void LogEnemyKillEvent(const EnemyKillEvent& event) {
    if (!IsDebugEnabled()) {
        return;
    }

    std::printf("[TrueCoop] EnemyKillEvent %s\n", DescribeEnemyKillEvent(event).c_str());
}

void LogEnemyTransformEvent(const EnemyTransformEvent& event) {
    if (!IsDebugEnabled()) {
        return;
    }

    std::printf("[TrueCoop] EnemyTransformEvent %s\n", DescribeEnemyTransformEvent(event).c_str());
}

void LogEnemyStateEvent(const EnemyStateEvent& event) {
    if (!IsDebugEnabled()) {
        return;
    }

    std::printf("[TrueCoop] EnemyStateEvent %s\n", DescribeEnemyStateEvent(event).c_str());
}

} // namespace TrueCoop
