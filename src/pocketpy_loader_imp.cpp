#include "loader_info.hpp"
#include "logger.h"
#include "pocketpy_loader.h"
#include "pocketpy_plugin_loader.hpp"

#include <ll/api/Logger.h>
#include <ll/api/plugin/NativePlugin.h>
#include <ll/api/plugin/PluginManagerRegistry.h>
#include <memory>
FILE_LOGGER();
namespace pocketpy_plugin_loader {

pocketpy_loader::pocketpy_loader() = default;

pocketpy_loader& pocketpy_loader::getInstance() {
    static pocketpy_loader instance;
    return instance;
}

ll::plugin::NativePlugin& pocketpy_loader::getSelf() const { return *mSelf; }

bool pocketpy_loader::load(ll::plugin::NativePlugin& self) {
    mSelf = std::addressof(self);
    logger.info("loading {}:{}", loader_info::loader_name, loader_info::version);
    auto res = ll::plugin::PluginManagerRegistry::getInstance().addManager(
        std::make_shared<::pocketpy_plugin_loader::pocketpy_plugin_manager>()
    );
    return res;
}

bool pocketpy_loader::enable() {
    logger.info("enabling {}:{}", loader_info::loader_name, loader_info::version);
    return true;
}

bool pocketpy_loader::disable() {
    logger.info("disabling {}:{}", loader_info::loader_name, loader_info::version);
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
}

} // namespace pocketpy_plugin_loader
