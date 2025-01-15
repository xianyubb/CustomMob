#pragma once
#include <optional>
#include <string>
#include <unordered_map>

namespace CustomMob {
struct Config {
    struct MobAttribute {
        std::optional<float> movementSpeed           = 0;
        std::optional<float> lavaMovementSpeed       = 0;
        std::optional<float> underWaterMovementSpeed = 0;
        std::optional<float> health                  = 0;
        std::optional<float> attackDamage            = 0;
        std::optional<float> followRange             = 0;
        std::optional<float> jumpStrength            = 0;
        std::optional<float> knockbackResistance     = 0;
        std::optional<float> absorption              = 0;
        std::optional<float> luck                    = 0;
        // std::optional<std::unordered_map<std::string, std::array<int, 2>>> loot                = {};
    };
    int                                           version = 1;
    std::unordered_map<std::string, MobAttribute> mobs    = {};
};
} // namespace CustomMob