#pragma once
#include <ll/api/plugin/PluginManager.h>
namespace pocketpy_plugin_loader {
class pocketpy_plugin_manager : public ::ll::plugin::PluginManager {
    struct Impl;
    Impl* data;

public:
    pocketpy_plugin_manager();
    pocketpy_plugin_manager(const pocketpy_plugin_manager&)            = delete;
    pocketpy_plugin_manager(pocketpy_plugin_manager&&)                 = delete;
    pocketpy_plugin_manager& operator=(const pocketpy_plugin_manager&) = delete;
    pocketpy_plugin_manager& operator=(pocketpy_plugin_manager&&)      = delete;
    ~pocketpy_plugin_manager() override;

    virtual bool load(ll::plugin::Manifest manifest) override;
    virtual bool unload(std::string_view name) override;
    virtual bool enable(std::string_view name) override;
    virtual bool disable(std::string_view name) override;
};
} // namespace pocketpy_plugin_loader
