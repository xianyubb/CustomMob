#pragma once
#include "Config.h"

#include <ll/api/event/ListenerBase.h>

#include <ll/api/mod/NativeMod.h>

namespace CustomMob {

class Entry {

public:
    static Entry& getInstance();

    Entry() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();

    bool enable();

    bool disable();

    bool unload();

    Config& getConfig() { return mConfig.value(); }

    void loadOrReloadConfig();

private:
    ll::mod::NativeMod&    mSelf;
    ll::event::ListenerPtr mSpawnedMobListener;
    std::optional<Config>  mConfig;
};

} // namespace CustomMob