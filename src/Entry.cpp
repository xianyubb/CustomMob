#include "Entry.h"
#include "ll/api/event/eventBus.h"
#include <ll/api/Config.h>
#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <ll/api/event/world/SpawnMobEvent.h>
#include <ll/api/mod/RegisterHelper.h>
#include <mc/server/commands/CommandOrigin.h>
#include <mc/server/commands/CommandOutput.h>
#include <mc/world/attribute/AttributeInstance.h>
#include <mc/world/attribute/SharedAttributes.h>

#define MODIFY_ATTRIBUTE(CONFIG_NAME, NAME)                                                                            \
    if (setting.CONFIG_NAME.has_value()) {                                                                             \
        if (auto& attribute = const_cast<AttributeInstance&>(event.mob()->getAttribute(SharedAttributes::NAME()));     \
            attribute.isValid()) {                                                                                     \
            attribute.setMaxValue(setting.CONFIG_NAME.value());                                                        \
            attribute.setCurrentValue(setting.CONFIG_NAME.value());                                                    \
        }                                                                                                              \
    }

namespace CustomMob {

Entry& Entry::getInstance() {
    static Entry instance;
    return instance;
}

bool Entry::load() {
    loadOrReloadConfig();

    mSpawnedMobListener = ll::event::EventBus::getInstance().emplaceListener<ll::event::SpawnedMobEvent>(
        [this](ll::event::SpawnedMobEvent& event) -> void {
            if (!event.mob().has_value() || !getConfig().mobs.contains(event.mob()->getTypeName())) return;
            auto& setting = getConfig().mobs.at(event.mob()->getTypeName());
            MODIFY_ATTRIBUTE(absorption, ABSORPTION);
            MODIFY_ATTRIBUTE(movementSpeed, MOVEMENT_SPEED);
            MODIFY_ATTRIBUTE(lavaMovementSpeed, LAVA_MOVEMENT_SPEED);
            MODIFY_ATTRIBUTE(underWaterMovementSpeed, UNDERWATER_MOVEMENT_SPEED);
            MODIFY_ATTRIBUTE(health, HEALTH);
            MODIFY_ATTRIBUTE(attackDamage, ATTACK_DAMAGE);
            MODIFY_ATTRIBUTE(followRange, FOLLOW_RANGE);
            MODIFY_ATTRIBUTE(jumpStrength, JUMP_STRENGTH);
            MODIFY_ATTRIBUTE(knockbackResistance, KNOCKBACK_RESISTANCE);
            MODIFY_ATTRIBUTE(luck, LUCK);
        }
    );

    return true;
}

bool Entry::enable() {
    auto& cmd = ll::command::CommandRegistrar::getInstance()
                    .getOrCreateCommand("custommob", "重载", CommandPermissionLevel::GameDirectors);
    cmd.overload().execute([this](CommandOrigin const&, CommandOutput& output) -> void {
        loadOrReloadConfig();
        output.success("[CustomMob] 已尝试重载配置文件");
    });
    return true;
}

bool Entry::disable() {
    ll::event::EventBus::getInstance().removeListener(mSpawnedMobListener);
    mConfig.reset();
    return true;
}

bool Entry::unload() { return true; }

void Entry::loadOrReloadConfig() {
    auto const& path = getSelf().getConfigDir() / "config.json";
    try {
        if (!mConfig.has_value()) mConfig.emplace();
        ll::config::loadConfig(*mConfig, path);
    } catch (...) {}
    ll::config::saveConfig(*mConfig, path);
}

} // namespace CustomMob

LL_REGISTER_MOD(CustomMob::Entry, CustomMob::Entry::getInstance())