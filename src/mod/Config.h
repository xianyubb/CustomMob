#pragma once

#include <map>
#include <optional>
#include <string>


using std::optional;

typedef optional<float> oFloat;

struct Speed {
    oFloat movementSpeed           = 0;
    oFloat lavaMovementSpeed       = 0;
    oFloat underWaterMovementSpeed = 0;
};


struct MobAttributes {
    oFloat          health              = 0;
    oFloat          attackDamage        = 0;
    optional<Speed> speed               = {};
    oFloat          followRange         = 0;
    oFloat          jumpStrength        = 0;
    oFloat          knockbackResistance = 0;
    oFloat          absorption          = 0;
    oFloat          luck                = 0;
};

struct Config {
    int                                  version = 1;
    std::map<std::string, MobAttributes> mobs    = {};
};
