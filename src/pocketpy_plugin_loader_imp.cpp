#include "logger.h"
#include "pocketpy_plugin_loader.hpp"
FILE_LOGGER();
struct ::pocketpy_plugin_loader::pocketpy_plugin_manager::Impl {
    [[maybe_unused]] static pocketpy_plugin_loader::pocketpy_plugin_manager*
    data(pocketpy_plugin_loader::pocketpy_plugin_manager* ptr = nullptr) {
        static pocketpy_plugin_loader::pocketpy_plugin_manager* data = nullptr;
        if (ptr) data = ptr;
        return data;
    }
};
pocketpy_plugin_loader::pocketpy_plugin_manager::pocketpy_plugin_manager()
: ::ll::plugin::PluginManager("pocketpy-loader") {
    ::pocketpy_plugin_loader::pocketpy_plugin_manager::Impl::data(this);
}
pocketpy_plugin_loader::pocketpy_plugin_manager::~pocketpy_plugin_manager() {}

// extern static bool ::pocketpy_plugin_loader::engine::load(ll::plugin::Manifest manifest);
namespace pocketpy_plugin_loader {
namespace engine {
extern bool load(ll::plugin::Manifest manifest);
extern bool unload(std::string_view name);
extern bool enable(std::string_view name);
extern bool disable(std::string_view name);
} // namespace engine
} // namespace pocketpy_plugin_loader


bool ::pocketpy_plugin_loader::pocketpy_plugin_manager::load(ll::plugin::Manifest manifest) {
    return ::pocketpy_plugin_loader::engine::load(manifest);
}

bool ::pocketpy_plugin_loader::pocketpy_plugin_manager::unload(std::string_view name) {
    return ::pocketpy_plugin_loader::engine::unload(name);
}

bool ::pocketpy_plugin_loader::pocketpy_plugin_manager::enable(std::string_view name) {
    return ::pocketpy_plugin_loader::engine::enable(name);
}

bool ::pocketpy_plugin_loader::pocketpy_plugin_manager::disable(std::string_view name) {
    return ::pocketpy_plugin_loader::engine::disable(name);
}

std::shared_ptr<ll::plugin::Plugin> pocketpy_plugin_loader::pocketpy_plugin_manager::get(std::string_view name) {
    // return ::pocketpy_plugin_loader::engine::disable(name);
    auto manager = ::pocketpy_plugin_loader::pocketpy_plugin_manager::Impl::data();
    return manager->getPlugin(name); //
}
