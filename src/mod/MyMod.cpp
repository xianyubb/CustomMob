#include "mod/MyMod.h"
#include "mod/Config.h"

#include "ll/api/Config.h"
#include "ll/api/Logger.h"
#include "ll/api/command/CommandHandle.h"
#include "ll/api/command/CommandRegistrar.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/world/SpawnMobEvent.h"
#include "ll/api/mod/RegisterHelper.h"


#include "mc/server/commands/CommandOrigin.h"
#include "mc/server/commands/CommandOutput.h"
#include "mc/world/actor/Mob.h"
#include "mc/world/attribute/AttributeInstance.h"
#include "mc/world/attribute/SharedAttributes.h"


#include <memory>
#include <string>

namespace {
Config config;
};

bool stringInclude(const std::string& str, const std::string& substr) { return str.find(substr) != std::string::npos; }

void enableMod() {
    ll::Logger logger("CustomMob");
    if (config.mobs.empty()) {
        logger.warn("请添加生物属性...");
    }

    for (auto& mob : config.mobs) {
        if (!stringInclude(mob.first, "minecraft:")) {
            config.mobs["minecraft:" + mob.first] = mob.second;
            config.mobs.erase(mob.first);
        }
    }
    ll::config::saveConfig(config, my_mod::MyMod::getInstance().getSelf().getConfigDir() / "config.json");
}

void reloadConfig() {
    ll::Logger logger("CustomMob");
    logger.info("重载配置文件中...");
    const auto& configFilePath = my_mod::MyMod::getInstance().getSelf().getConfigDir() / "config.json";
    if (!ll::config::loadConfig(config, configFilePath)) {
        logger.warn("Cannot load configurations from {}", configFilePath);
        logger.info("Saving default configurations");
    }
    // ll::config::saveConfig(config, configFilePath);
    enableMod();
}


void enableForMob(Mob& mob) {
    if (config.mobs.empty()) {
        return;
    }
    const auto& mobAttributes = config.mobs.find(mob.getTypeName());
    if (mobAttributes != config.mobs.end()) {
        const auto& attributes = mobAttributes->second;
        if (attributes.speed.has_value()) {
            if (mob.getMutableAttribute(SharedAttributes::MOVEMENT_SPEED)->isValid()
                && attributes.speed->movementSpeed.has_value()) {
                mob.getMutableAttribute(SharedAttributes::MOVEMENT_SPEED)
                    ->setMaxValue(attributes.speed->movementSpeed.value());
                mob.getMutableAttribute(SharedAttributes::MOVEMENT_SPEED)
                    ->setCurrentValue(attributes.speed->movementSpeed.value());
            }
            if (mob.getMutableAttribute(SharedAttributes::LAVA_MOVEMENT_SPEED)->isValid()
                && attributes.speed->lavaMovementSpeed.has_value()) {
                mob.getMutableAttribute(SharedAttributes::LAVA_MOVEMENT_SPEED)
                    ->setMaxValue(attributes.speed->lavaMovementSpeed.value());
                mob.getMutableAttribute(SharedAttributes::LAVA_MOVEMENT_SPEED)
                    ->setCurrentValue(attributes.speed->lavaMovementSpeed.value());
            }
            if (mob.getMutableAttribute(SharedAttributes::UNDERWATER_MOVEMENT_SPEED)->isValid()
                && attributes.speed->underWaterMovementSpeed.has_value()) {
                mob.getMutableAttribute(SharedAttributes::UNDERWATER_MOVEMENT_SPEED)
                    ->setMaxValue(attributes.speed->underWaterMovementSpeed.value());
                mob.getMutableAttribute(SharedAttributes::UNDERWATER_MOVEMENT_SPEED)
                    ->setCurrentValue(attributes.speed->underWaterMovementSpeed.value());
            }
        }
        if (mob.getMutableAttribute(SharedAttributes::UNDERWATER_MOVEMENT_SPEED)->isValid()
            && attributes.health.has_value()) {
            mob.getMutableAttribute(SharedAttributes::HEALTH)->setMaxValue(attributes.health.value());
            mob.getMutableAttribute(SharedAttributes::HEALTH)->setCurrentValue(attributes.health.value());
        }
        if (mob.getMutableAttribute(SharedAttributes::ATTACK_DAMAGE)->isValid()
            && attributes.attackDamage.has_value()) {
            mob.getMutableAttribute(SharedAttributes::ATTACK_DAMAGE)->setMaxValue(attributes.attackDamage.value());
            mob.getMutableAttribute(SharedAttributes::ATTACK_DAMAGE)->setCurrentValue(attributes.attackDamage.value());
        }
        if (mob.getMutableAttribute(SharedAttributes::ABSORPTION)->isValid() && attributes.absorption.has_value()) {
            mob.getMutableAttribute(SharedAttributes::ABSORPTION)->setMaxValue(attributes.absorption.value());
            mob.getMutableAttribute(SharedAttributes::ABSORPTION)->setCurrentValue(attributes.absorption.value());
        }
        if (mob.getMutableAttribute(SharedAttributes::KNOCKBACK_RESISTANCE)->isValid()
            && attributes.knockbackResistance.has_value()) {
            mob.getMutableAttribute(SharedAttributes::KNOCKBACK_RESISTANCE)
                ->setMaxValue(attributes.knockbackResistance.value());
            mob.getMutableAttribute(SharedAttributes::KNOCKBACK_RESISTANCE)
                ->setCurrentValue(attributes.knockbackResistance.value());
        }
        if (mob.getMutableAttribute(SharedAttributes::LUCK)->isValid() && attributes.luck.has_value()) {
            mob.getMutableAttribute(SharedAttributes::LUCK)->setMaxValue(attributes.luck.value());
            mob.getMutableAttribute(SharedAttributes::LUCK)->setCurrentValue(attributes.luck.value());
        }
        if (mob.getMutableAttribute(SharedAttributes::FOLLOW_RANGE)->isValid() && attributes.followRange.has_value()) {
            mob.getMutableAttribute(SharedAttributes::FOLLOW_RANGE)->setMaxValue(attributes.followRange.value());
            mob.getMutableAttribute(SharedAttributes::FOLLOW_RANGE)->setCurrentValue(attributes.followRange.value());
        }
        if (mob.getMutableAttribute(SharedAttributes::JUMP_STRENGTH)->isValid()
            && attributes.jumpStrength.has_value()) {
            mob.getMutableAttribute(SharedAttributes::JUMP_STRENGTH)->setMaxValue(attributes.jumpStrength.value());
            mob.getMutableAttribute(SharedAttributes::JUMP_STRENGTH)->setCurrentValue(attributes.jumpStrength.value());
        }
    }
}

void regReloadCommand() {
    auto& cmdBus = ll::command::CommandRegistrar::getInstance();
    auto& cmd    = cmdBus.getOrCreateCommand("custommob", "重载", CommandPermissionLevel::GameDirectors);
    cmd.alias("cm");
    cmd.overload().execute([](CommandOrigin const& origin, CommandOutput& output) {
        if (origin.getOriginType() == CommandOriginType::DedicatedServer) {
            reloadConfig();
            output.success("[CustomMob] 重载配置文件成功");
            return;
        };
        auto entity = origin.getEntity();
        if (entity == nullptr || !entity->isType(ActorType::Player)) {
            output.error("[CustomMob] 此命令只能玩家或者控制台使用");
            return;
        }
        reloadConfig();
        output.success("[CustomMob] 重载配置文件成功");
    });
}

namespace my_mod {

static std::unique_ptr<MyMod> instance;

MyMod& MyMod::getInstance() { return *instance; }

bool MyMod::load() {
    getSelf().getLogger().debug("Loading...");
    auto&       logger         = getSelf().getLogger();
    const auto& configFilePath = getSelf().getConfigDir() / "config.json";
    if (!ll::config::loadConfig(config, configFilePath)) {
        logger.warn("Cannot load configurations from {}", configFilePath);
        logger.info("Saving default configurations");
        if (!ll::config::saveConfig(config, configFilePath)) {
            logger.error("Cannot save default configurations to {}", configFilePath);
        }
    };
    return true;
    // Code for loading the mod goes here.
    return true;
}

bool MyMod::enable() {
    getSelf().getLogger().debug("Enabling...");
    // Code for enabling the mod goes here.
    enableMod();
    regReloadCommand();
    auto& eventBus = ll::event::EventBus::getInstance();

    eventBus.emplaceListener<ll::event::SpawnedMobEvent>([](ll::event::SpawnedMobEvent& event) -> bool {
        auto mob = event.mob();
        enableForMob(mob);
        return true;
    });

    return true;
}

bool MyMod::disable() {
    getSelf().getLogger().debug("Disabling...");
    // Code for disabling the mod goes here.
    return true;
}

} // namespace my_mod

LL_REGISTER_MOD(my_mod::MyMod, my_mod::instance);
