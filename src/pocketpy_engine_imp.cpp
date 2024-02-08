#include "loader_info.hpp"
#include "logger.h"
#include "pocketpy.h"
#include "pocketpy_type_binds.h"
#include "type_wappers/actor_wapper.h"
#include "type_wappers/listener_result_wapper.h"

#include <filesystem>
#include <ll/api/event/EventBus.h>
#include <ll/api/event/entity/ActorEvent.h>
#include <ll/api/event/entity/ActorHurtEvent.h>
#include <ll/api/event/server/ServerStartedEvent.h>
#include <ll/api/event/server/ServerStoppingEvent.h>
#include <ll/api/memory/Hook.h>
#include <ll/api/plugin/Manifest.h>
#include <ll/api/plugin/PluginManager.h>
#include <ll/api/service/ServerInfo.h>
#include <magic_enum.hpp>
#include <mc/server/ServerInstance.h>
#include <vector>


FILE_LOGGER();

namespace pocketpy_plugin_loader {
namespace engine {
struct pocketpy_engine {
    pkpy::VM* vm;
    PyObject* on_enable;
    PyObject* on_disable;
    PyObject* on_load;
    PyObject* on_unload;
    pocketpy_engine() {
        vm         = nullptr;
        on_enable  = nullptr;
        on_disable = nullptr;
        on_load    = nullptr;
        on_unload  = nullptr;
    }
    ~pocketpy_engine() { delete vm; }
    pocketpy_engine(const pocketpy_engine&) = delete;
    pocketpy_engine(pocketpy_engine&& o) { ::std::swap(vm, o.vm); }
    pocketpy_engine& operator=(pocketpy_engine&& o) {
        ::std::swap(vm, o.vm);
        return *this;
    }
};
using engines_t = ::std::unordered_map<std::string, pocketpy_engine>;
engines_t         engines;
std::vector<char> readF(auto path) {
    auto              f = fopen(path, "r");
    std::vector<char> source;
    char              c = getc(f);
    while (c != EOF) {
        source.push_back(c);
        c = getc(f);
    }
    fclose(f);
    return source;
}
struct {
    std::string name;
} current_loader_context;
// void setupTypeBinds(VM*);
void setupLoaderModule(VM* vm) {
    auto loaderModule = vm->new_module("LoaderApi");
    vm->bind(loaderModule, "getPocketpyLoaderVersion()->str", [](VM* vm, ArgsView) {
        return py_var(vm, loader_info::version);
    });
    vm->bind(loaderModule, "getLoaderVersion()->str", [](VM* vm, ArgsView) {
        return py_var(vm, ll::getLoaderVersion().to_string());
    });
    vm->bind(loaderModule, "getBdsVersion()->str", [](VM* vm, ArgsView) {
        return py_var(vm, ll::getBdsVersion().to_string());
    });
    vm->bind(loaderModule, "getServerProtocolVersion()->int", [](VM* vm, ArgsView) {
        return py_var(vm, ll::getServerProtocolVersion());
    });
    vm->bind(loaderModule, "getServerServerStatus()->str", [](VM* vm, ArgsView) {
        auto s = ll::getServerStatus();
        // logger.info(magic_enum::enum_name(s));
        return py_var(vm, magic_enum::enum_name(s));
    });
    vm->bind(loaderModule, "registerOnEnableCallback(callback:function)->None", [](VM* vm, ArgsView args) {
        engines[current_loader_context.name].on_enable = args[0];
        return py_var(vm, pkpy::NoReturn{});
    });
    vm->bind(loaderModule, "registerOnDisableCallback(callback:function)->None", [](VM* vm, ArgsView args) {
        engines[current_loader_context.name].on_disable = args[0];
        return py_var(vm, pkpy::NoReturn{});
    });
    vm->bind(loaderModule, "registerOnLoadCallback(callback:function)->None", [](VM* vm, ArgsView args) {
        engines[current_loader_context.name].on_load = args[0];
        return py_var(vm, pkpy::NoReturn{});
    });
    vm->bind(loaderModule, "registerOnUnloadCallback(callback:function)->None", [](VM* vm, ArgsView args) {
        engines[current_loader_context.name].on_unload = args[0];
        return py_var(vm, pkpy::NoReturn{});
    });
}

// struct ListenerResultWapper {
//     static void _register(VM* vm, PyObject* mod, PyObject* type) {
//         vm->bind__eq__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* lhs, PyObject* rhs) {
//             if (!vm->isinstance(rhs, ListenerResultWapper::_type(vm))) return vm->NotImplemented;
//             return VAR(
//                 PK_OBJ_GET(ListenerResultWapper, lhs).listener == PK_OBJ_GET(ListenerResultWapper, rhs).listener
//             );
//         });
//         vm->bind__hash__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
//             auto& self = PK_OBJ_GET(ListenerResultWapper, obj);
//             return (i64)(std::hash<std::shared_ptr<ll::event::ListenerBase>>{}(self.listener));
//         });
//         vm->bind__repr__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
//             ListenerResultWapper& self = PK_OBJ_GET(ListenerResultWapper, obj);
//             pkpy::SStream         ss;
//             ss.write_hex(std::addressof(*self.listener));
//             return VAR(_S("<ListenerResultWapper at ", ss.str(), ">"));
//         });
//         vm->bind_method<0>(type, "getEventId", [](VM* vm, ArgsView args) {
//             ListenerResultWapper& self = _CAST(ListenerResultWapper&, args[0]);
//             return VAR(self.listener->getId());
//         });
//         vm->bind_method<0>(type, "getPriority", [](VM* vm, ArgsView args) {
//             ListenerResultWapper& self = _CAST(ListenerResultWapper&, args[0]);
// #define RT_IF(P) \
//     if (self.listener->getPriority() == ll::event::EventPriority::##P) return VAR(#P)
//             RT_IF(Highest);
//             RT_IF(High);
//             RT_IF(Normal);
//             RT_IF(Low);
//             RT_IF(Lowest);
//             return VAR("Unknown");
// #undef RT_IF
//         });
//     }
//     PY_CLASS(ListenerResultWapper, EventsApi, ListenerResultWapper)
//     std::shared_ptr<ll::event::ListenerBase> listener;
//     ListenerResultWapper(auto l) { listener = l; }
//     ~ListenerResultWapper() {
//         // puts("on de");
//         ll::event::EventBus::getInstance().removeListener(listener->getId());
//     }
// };
void setupEvevntsModule(VM* vm) {
    auto eventsModule = vm->new_module("EventsApi");
    type_wappers::ListenerResultWapper::register_class(vm, eventsModule);
    vm->bind(eventsModule, "onServerStartedEvent(fn:function)", [](VM* vm, ArgsView args) {
        auto& bus      = ll::event::EventBus::getInstance();
        auto  callback = args[0];
        auto res = bus.emplaceListener<ll::event::ServerStartedEvent>([vm, callback](auto& ev) { vm->call(callback); });
        type_wappers::ListenerResultWapper wapper(res);
        return VAR_T(type_wappers::ListenerResultWapper, wapper);
    });
    vm->bind(eventsModule, "onServerStopingEvent(fn:function)", [](VM* vm, ArgsView args) {
        auto& bus      = ll::event::EventBus::getInstance();
        auto  callback = args[0];
        auto  res =
            bus.emplaceListener<ll::event::ServerStoppingEvent>([vm, callback](auto& ev) { vm->call(callback); });
        type_wappers::ListenerResultWapper wapper(res);
        return VAR_T(type_wappers::ListenerResultWapper, wapper);
    });
    vm->bind(eventsModule, "onActorHurtEvent(fn:function)", [](VM* vm, ArgsView args) {
        auto& bus      = ll::event::EventBus::getInstance();
        auto  callback = args[0];
        auto  res      = bus.emplaceListener<ll::event::entity::ActorHurtEvent>([vm, callback](auto& ev) {
            puts("on hurt");
            type_wappers::ActorWapper wapper(&ev.self());
            vm->call(callback, VAR_T(type_wappers::ActorWapper, wapper));
        });
        type_wappers::ListenerResultWapper wapper(res);
        return VAR_T(type_wappers::ListenerResultWapper, wapper);
    });
}

VM* setupVM() {
    auto vm = new VM();
    setupLoaderModule(vm);
    setupEvevntsModule(vm);
    setupTypeBinds(vm);
    return vm;
}
#define CALL_LOADER_CALLBACK(CALLBACK)                                                                                 \
    [](auto& engine) {                                                                                                 \
        if (!engine.on_##CALLBACK) return true;                                                                        \
        VM*       vm  = engine.vm;                                                                                     \
        PyObject* obj = engine.on_##CALLBACK;                                                                          \
        auto      res = vm->call(obj);                                                                                 \
        if (vm->py_bool(obj)) return true;                                                                             \
        else return false;                                                                                             \
    }

bool load(ll::plugin::Manifest manifest) {
    if (engines.contains(manifest.name)) {
        logger.info("plugin : {} load twice", manifest.name);
        return false;
    }
    // context.path = "./plugins/" + manifest.name + "/" + manifest.entry;
    // context.dir  = "./plugins/" + manifest.name;
    // puts(("./plugins/" + manifest.name + "/" + manifest.entry).c_str());
    if (!::std::filesystem::exists("./plugins/" + manifest.name + "/" + manifest.entry)) {
        logger.info("plugin : {} not exists", manifest.name);
        return false;
    }
    if (!(::std::filesystem::status("./plugins/" + manifest.name + "/" + manifest.entry).type()
          == std::filesystem::file_type::regular))
        return false;
    pocketpy_engine engine{};
    current_loader_context.name = manifest.name;
    engine.vm                   = setupVM();
    auto file                   = readF(("./plugins/" + manifest.name + "/" + manifest.entry).c_str());
    engine.vm->exec(
        {file.data(), (int)file.size()},
        "./plugins/" + manifest.name + "/" + manifest.entry,
        pkpy::CompileMode::EXEC_MODE
    );
    engines[manifest.name] = std::move(engine);
    return CALL_LOADER_CALLBACK(load)(engines[manifest.name]);
}

bool unload(std::string_view name) { return CALL_LOADER_CALLBACK(unload)(engines[std::string(name)]); }

bool enable(std::string_view name) { return CALL_LOADER_CALLBACK(enable)(engines[std::string(name)]); }

bool disable(std::string_view name) { return CALL_LOADER_CALLBACK(disable)(engines[std::string(name)]); }
} // namespace engine
#undef CALL_LOADER_CALLBACK
} // namespace pocketpy_plugin_loader
