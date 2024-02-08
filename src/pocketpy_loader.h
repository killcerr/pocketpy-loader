#pragma once

#include <ll/api/plugin/NativePlugin.h>

namespace pocketpy_plugin_loader {

class pocketpy_loader {
    pocketpy_loader();

public:
    pocketpy_loader(pocketpy_loader&&)                 = delete;
    pocketpy_loader(const pocketpy_loader&)            = delete;
    pocketpy_loader& operator=(pocketpy_loader&&)      = delete;
    pocketpy_loader& operator=(const pocketpy_loader&) = delete;

    static pocketpy_loader& getInstance();

    [[nodiscard]] ll::plugin::NativePlugin& getSelf() const;

    /// @return True if the plugin is loaded successfully.
    bool load(ll::plugin::NativePlugin&);

    /// @return True if the plugin is enabled successfully.
    bool enable();

    /// @return True if the plugin is disabled successfully.
    bool disable();

private:
    ll::plugin::NativePlugin* mSelf{};
};

} // namespace pocketpy_plugin_loader
