#include "pocketpy_loader.h"

#include "pocketpy_plugin_loader.hpp"

#include <ll/api/plugin/NativePlugin.h>
#include <ll/api/plugin/PluginManagerRegistry.h>
#include <memory>

namespace pocketpy_plugin_loader {

pocketpy_loader::pocketpy_loader() = default;

pocketpy_loader& pocketpy_loader::getInstance() {
    static pocketpy_loader instance;
    return instance;
}

ll::plugin::NativePlugin& pocketpy_loader::getSelf() const { return *mSelf; }

bool pocketpy_loader::load(ll::plugin::NativePlugin& self) {
    mSelf = std::addressof(self);
    getSelf().getLogger().info("loading...");
    auto res = ll::plugin::PluginManagerRegistry::getInstance().addManager(
        std::make_shared<::pocketpy_plugin_loader::pocketpy_plugin_manager>()
    );
    return true;
}

bool pocketpy_loader::enable() {
    getSelf().getLogger().info("enabling...");

    return true;
}

bool pocketpy_loader::disable() {
    getSelf().getLogger().info("disabling...");

    // Code for disabling the plugin goes here.

    return true;
}

extern "C" {
_declspec(dllexport) bool ll_plugin_load(ll::plugin::NativePlugin& self) {
    return pocketpy_loader::getInstance().load(self);
}

_declspec(dllexport) bool ll_plugin_enable(ll::plugin::NativePlugin&) {
    return pocketpy_loader::getInstance().enable();
}

_declspec(dllexport) bool ll_plugin_disable(ll::plugin::NativePlugin&) {
    return pocketpy_loader::getInstance().disable();
}

/// @warning Unloading the plugin may cause a crash if the plugin has not released all of its
/// resources. If you are unsure, keep this function commented out.
// _declspec(dllexport) bool ll_plugin_unload(ll::plugin::NativePlugin&) {
//     return test_prject::getInstance().unload();
// }
}

} // namespace pocketpy_plugin_loader
