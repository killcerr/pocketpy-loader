#include "loader_info.hpp"
#include "logger.h"
#include "native_helper.h"
#include "pocketpy.h"
#include "pocketpy_plugin_loader.hpp"
#include "pocketpy_type_binds.h"
#include "type_wappers/actor_wapper.h"
#include "type_wappers/listener_result_wapper.h"

#include <filesystem>
#include <ll/api/event/DynamicListener.h>
#include <ll/api/event/EventBus.h>
#include <ll/api/event/command/ExecuteCommandEvent.h>
#include <ll/api/event/entity/ActorEvent.h>
#include <ll/api/event/entity/ActorHurtEvent.h>
#include <ll/api/event/entity/MobDieEvent.h>
#include <ll/api/event/player/PlayerAddExperienceEvent.h>
#include <ll/api/event/player/PlayerAttackEvent.h>
#include <ll/api/event/player/PlayerChangePermEvent.h>
#include <ll/api/event/player/PlayerChatEvent.h>
#include <ll/api/event/player/PlayerClickEvent.h>
#include <ll/api/event/player/PlayerConnectEvent.h>
#include <ll/api/event/player/PlayerDestroyBlockEvent.h>
#include <ll/api/event/player/PlayerDieEvent.h>
#include <ll/api/event/player/PlayerInteractBlockEvent.h>
#include <ll/api/event/player/PlayerJoinEvent.h>
#include <ll/api/event/player/PlayerJumpEvent.h>
#include <ll/api/event/player/PlayerLeaveEvent.h>
#include <ll/api/event/player/PlayerPickUpItemEvent.h>
#include <ll/api/event/player/PlayerPlaceBlockEvent.h>
#include <ll/api/event/player/PlayerRespawnEvent.h>
#include <ll/api/event/player/PlayerSneakEvent.h>
#include <ll/api/event/player/PlayerSprintEvent.h>
#include <ll/api/event/player/PlayerSwingEvent.h>
#include <ll/api/event/player/PlayerUseItemEvent.h>
#include <ll/api/event/player/PlayerUseItemOnEvent.h>
#include <ll/api/event/server/ServerStartedEvent.h>
#include <ll/api/event/server/ServerStoppingEvent.h>
#include <ll/api/event/server/ServiceEvents.h>
#include <ll/api/event/world/BlockChangedEvent.h>
#include <ll/api/event/world/FireSpreadEvent.h>
#include <ll/api/event/world/SpawnMobEvent.h>
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
    auto f = fopen(path, "r");
    // logger.info(path);
    if (!f) return {};
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

PyObject* CompoundTagToDict(VM* vm, const CompoundTag& nbt) {
    auto json = nbt.toSnbt(SnbtFormat::Jsonify);
    // logger.info(json);
    std::string p;
    bool        inComment = false;
    for (::std::size_t i = 0; i < json.size(); i++) {
        if (json[i] == '/' && i + 1 < json.size() && json[i + 1] == '*') inComment = true;
        if (i - 1 >= 0 && json[i - 1] == '/' && i - 2 >= 0 && json[i - 2] == '*') inComment = false;
        if (!inComment) p += json[i];
    }
    // pkpy::JsonSerializer(vm, root)
    auto res = vm->call(vm->_modules["json"]->attr("loads"), py_var(vm, p));
    return res;
}

thread_local struct {
    CompoundTag*             nbt;
    std::vector<std::string> path;
    std::string              type;
    VM*                      vm;
    PyObject*                val;
} nbtSetter_context;

bool nbtSetter() {
    if (nbtSetter_context.path.size() == 1) {
        if (nbtSetter_context.type == "EndTag") {
            (*nbtSetter_context.nbt)[nbtSetter_context.path[0]] = EndTag{};
            return true;
        }
        if (nbtSetter_context.type == "ByteTag") {

            (*nbtSetter_context.nbt)[nbtSetter_context.path[0]] =
                ByteTag{py_cast<char>(nbtSetter_context.vm, nbtSetter_context.val)};
            return true;
        }
        if (nbtSetter_context.type == "ShortTag") {
            (*nbtSetter_context.nbt)[nbtSetter_context.path[0]] =
                ShortTag{py_cast<short>(nbtSetter_context.vm, nbtSetter_context.val)};
            return true;
        }
        if (nbtSetter_context.type == "IntTag") {
            (*nbtSetter_context.nbt)[nbtSetter_context.path[0]] =
                IntTag{py_cast<int>(nbtSetter_context.vm, nbtSetter_context.val)};
            return true;
        }
        if (nbtSetter_context.type == "Int64Tag") {
            (*nbtSetter_context.nbt)[nbtSetter_context.path[0]] =
                IntTag{py_cast<i64>(nbtSetter_context.vm, nbtSetter_context.val)};
            return true;
        }
        if (nbtSetter_context.type == "FloatTag") {
            (*nbtSetter_context.nbt)[nbtSetter_context.path[0]] =
                FloatTag{py_cast<float>(nbtSetter_context.vm, nbtSetter_context.val)};
            return true;
        }
        if (nbtSetter_context.type == "DoubleTag") {
            (*nbtSetter_context.nbt)[nbtSetter_context.path[0]] =
                DoubleTag{py_cast<double>(nbtSetter_context.vm, nbtSetter_context.val)};
            return true;
        }
        if (nbtSetter_context.type == "ByteArrayTag") {
            auto&              t = _py_cast<List&>(nbtSetter_context.vm, nbtSetter_context.val);
            std::vector<schar> arr(t.size());
            for (auto c : t) {
                arr.push_back(py_cast<char>(nbtSetter_context.vm, c));
            }
            (*nbtSetter_context.nbt)[nbtSetter_context.path[0]] = ByteArrayTag{arr};
            return true;
        }
        if (nbtSetter_context.type == "StringTag") {
            (*nbtSetter_context.nbt)[nbtSetter_context.path[0]] =
                StringTag{py_cast<Str&>(nbtSetter_context.vm, nbtSetter_context.val).sv()};
            return true;
        }
        if (nbtSetter_context.type == "IntArrayTag") {
            auto&            t = _py_cast<List&>(nbtSetter_context.vm, nbtSetter_context.val);
            std::vector<int> arr(t.size());
            for (auto c : t) {
                arr.push_back(py_cast<int>(nbtSetter_context.vm, c));
            }
            (*nbtSetter_context.nbt)[nbtSetter_context.path[0]] = IntArrayTag{arr};
            return true;
        }
        if (nbtSetter_context.type == "CompoundTag") {
            auto& t     = _py_cast<Dict&>(nbtSetter_context.vm, nbtSetter_context.val);
            auto  store = nbtSetter_context.nbt;
            auto  lp    = nbtSetter_context.path[0];
            for (auto i : t.keys()) {
                if (t.try_get(i)->type == nbtSetter_context.vm->tp_tuple) {
                    auto p  = py_cast<List&>(nbtSetter_context.vm, i);
                    auto tp = py_cast<Tuple&>(nbtSetter_context.vm, t.try_get(i));
                    if (tp.size() != 2) {
                        nbtSetter_context.vm->RuntimeError("Tuple size must be 2");
                        continue;
                    }
                    auto type             = py_cast<Str&>(nbtSetter_context.vm, tp[0]);
                    auto val              = tp[1];
                    nbtSetter_context.nbt = store->get(lp)->as_ptr<CompoundTag>();
                    nbtSetter_context.path.clear();
                    nbtSetter_context.val  = val;
                    nbtSetter_context.type = type.c_str();
                    for (auto j : p) {
                        nbtSetter_context.path.push_back(py_cast<Str&>(nbtSetter_context.vm, j).c_str());
                    }
                    if (!nbtSetter()) return false;
                }
            }
        }
        return false;
    } else {
        auto store            = nbtSetter_context.nbt;
        nbtSetter_context.nbt = nbtSetter_context.nbt->get(nbtSetter_context.path[0])->as_ptr<CompoundTag>();
        nbtSetter_context.path.erase(nbtSetter_context.path.begin());
        auto res              = nbtSetter();
        nbtSetter_context.nbt = store;
        return res;
    }
}

void setupEvevntsModule(VM* vm) {
    auto playerEventsModule  = vm->new_module("PlayerEventsApi");
    auto entityEventsModule  = vm->new_module("EntityEventsApi");
    auto worldEventsModule   = vm->new_module("WorldEventsApi");
    auto serverEventsModule  = vm->new_module("ServerEventsApi");
    auto commandEventsModule = vm->new_module("CommandEventsApi");
#define IF(P)                                                                                                          \
    if (priority == #P) lpri = ll::event::EventPriority::##P
#define EVENT(P, N)                                                                                                    \
    vm->bind(P##EventsModule, "on" #N "(callback:function,priority:str)", [](VM* vm, ArgsView args) {                  \
        auto callback = args[0];                                                                                       \
        auto pl       = pocketpy_plugin_loader::pocketpy_plugin_manager::get(current_loader_context.name);             \
        logger.info(current_loader_context.name);                                                                      \
        auto                     priority = _CAST(Str&, args[1]);                                                      \
        ll::event::EventPriority lpri;                                                                                 \
        IF(Highest);                                                                                                   \
        IF(High);                                                                                                      \
        IF(Normal);                                                                                                    \
        IF(Low);                                                                                                       \
        IF(Lowest);                                                                                                    \
        auto listener = ll::event::DynamicListener::create(                                                            \
            [vm, callback](CompoundTag& nbt) {                                                                         \
                auto dict             = CompoundTagToDict(vm, nbt);                                                    \
                nbtSetter_context.nbt = &nbt;                                                                          \
                vm->call(callback, dict);                                                                              \
            },                                                                                                         \
            ll::event::EventPriority::Normal,                                                                          \
            pl                                                                                                         \
        );                                                                                                             \
        auto res = ll::event::EventBus::getInstance().addListener(listener, ll::event::getEventId<ll::event::##N>);    \
        type_wappers::ListenerResultWapper wapper(listener);                                                           \
        return VAR_T(type_wappers::ListenerResultWapper, wapper);                                                      \
    })
    EVENT(command, ExecuteCommandEvent);
    EVENT(entity, ActorHurtEvent);
    EVENT(entity, MobDieEvent);
    EVENT(player, PlayerAddExperienceEvent);
    EVENT(player, PlayerAttackEvent);
    EVENT(player, PlayerChangePermEvent);
    EVENT(player, PlayerChatEvent);
    EVENT(player, PlayerClickEvent);
    EVENT(player, PlayerConnectEvent);
    EVENT(player, PlayerDestroyBlockEvent);
    EVENT(player, PlayerDieEvent);
    EVENT(player, PlayerInteractBlockEvent);
    EVENT(player, PlayerJoinEvent);
    EVENT(player, PlayerJumpEvent);
    EVENT(player, PlayerLeaveEvent);
    EVENT(player, PlayerPickUpItemEvent);
    EVENT(player, PlayerPlaceBlockEvent);
    EVENT(player, PlayerRespawnEvent);
    EVENT(player, PlayerSneakEvent);
    EVENT(player, PlayerSprintEvent);
    EVENT(player, PlayerSwingEvent);
    EVENT(player, PlayerUseItemEvent);
    EVENT(player, PlayerUseItemOnEvent);
    EVENT(server, ServerStartedEvent);
    EVENT(server, ServerStoppingEvent);
    EVENT(server, ServiceRegisterEvent);
    EVENT(server, ServiceUnregisterEvent);
    EVENT(world, BlockChangedEvent);
    EVENT(world, FireSpreadEvent);
    EVENT(world, SpawnMobEvent);
#undef EVENT
}

void setupHelperModule(VM* vm) {
    auto helperModule = vm->new_module("Helper");
    vm->bind(helperModule, "setNbt(path:list,type:str,val)->bool", [](VM* vm, ArgsView args) {
        auto& p              = _py_cast<List&>(vm, args[0]);
        auto& t              = py_cast<Str&>(vm, args[1]);
        auto  v              = args[2];
        nbtSetter_context.vm = vm;
        nbtSetter_context.path.clear();
        for (auto pa : p) {
            nbtSetter_context.path.push_back(py_cast<Str&>(vm, pa).c_str());
        }
        nbtSetter_context.type = t.c_str();
        nbtSetter_context.val  = v;
        return VAR(nbtSetter());
    });
    vm->bind(helperModule, "call(symbol:str,ret:str,*args)", [](VM* vm, ArgsView args) {
        std::vector<::native_helper::ArgType>     argtypes;
        std::vector<::native_helper::TypeVariant> argvals;
        Tuple                                     _args = _CAST(Tuple, args[2]);
        for (auto arg : _args) {
            Tuple tv = _CAST(Tuple, arg);
            if (tv.size() != 2) {
                vm->RuntimeError("arg size must be 2");
                return VAR(NoReturn{});
            } else {
                auto type = ::native_helper::form_string(_CAST(Str&, tv[0]).sv());
                using enum ::native_helper::ArgType;
                using namespace native_helper;
                ::native_helper::TypeVariant va;
                /*logger.info("call_bind:{}", __LINE__);*/
                switch (type) {
#define CASE(N)                                                                                                        \
    case ui##N:                                                                                                        \
        /*logger.info("call_bind:{}", __LINE__);*/                                                                     \
        va.emplace<cast_to_int<ui##N>>(_CAST(uint##N##_t, tv[1]));                                                     \
        break;                                                                                                         \
    case i##N:                                                                                                         \
        /*logger.info("call_bind:{}", __LINE__);*/                                                                     \
        va.emplace<cast_to_int<ui##N>>(_CAST(uint##N##_t, tv[1]));                                                     \
        break
                case ui8:
                    va.emplace<cast_to_int<ui8>>(_py_cast<uint8_t>(vm, tv[1]));
                    break;
                case i8:
                    va.emplace<cast_to_int<i8>>(_py_cast<char>(vm, tv[1]));
                    break;
                    CASE(16);
                    CASE(32);
                    CASE(64);
#undef CASE
                case f:
                    // logger.info("call_bind:{}", __LINE__);
                    va.emplace<cast_to_int<f>>(_CAST(float, tv[1]));
                    break;
                case d:
                    // logger.info("call_bind:{}", __LINE__);
                    va.emplace<cast_to_int<d>>(_CAST(double, tv[1]));
                    break;
                case b:
                    // logger.info("call_bind:{}", __LINE__);
                    va.emplace<cast_to_int<b>>(_CAST(bool, tv[1]));
                    break;
                case c:
                    // logger.info("call_bind:{}", __LINE__);
                    va.emplace<cast_to_int<c>>(_CAST(char, tv[1]));
                    break;
                default:
                    logger.info("call_bind:{}", __LINE__);
                    break;
                }
                logger.info("call_bind:{}", __LINE__);
                argtypes.push_back(type);
                argvals.push_back(va);
            }
        }
        auto res = ::native_helper::call(
            ::native_helper::form_string(_CAST(Str&, args[1]).sv()),
            argtypes,
            argvals,
            ll::memory::resolveSymbol(_CAST(Str&, args[0]).sv(), false)
        );
        using enum ::native_helper::ArgType;
        using namespace ::native_helper;
        switch (::native_helper::form_string(_CAST(Str&, args[1]).sv())) {
        case v:
            return VAR(NoReturn{});
            break;
#define RT(N) return VAR(std::get<::native_helper::cast_to_int<N>>(res))
        case ui8:
            RT(ui8);
            break;
        case i8:
            RT(i8);
            break;
        case ui16:
            RT(ui16);
            break;
        case i16:
            RT(i16);
            break;
        case ui32:
            RT(ui32);
            break;
        case i32:
            RT(i32);
            break;
        case ui64:
            RT(ui64);
            break;
        case i64:
            RT(i64);
            break;
        case f:
            RT(f);
            break;
        case d:
            RT(d);
            break;
        case p:
            RT(p);
            break;
        case b:
            RT(b);
            break;
        case c:
            RT(c);
            break;
        default:
            vm->RuntimeError("unknown type");
            return VAR(NoReturn{});
            break;
        }
#undef RT
    });
}
struct {
    ::std::string current_import_path;
} import_context;
VM* setupVM() {
    auto vm             = new VM();
    vm->_import_handler = [](const char* name, int namesz, int* osize) -> unsigned char* {
        std::string f(name, namesz);
        for (::std::size_t i = 0; i < f.size(); i++) {
            if (f[i] == '\\') f[i] = '/';
        }
        auto fp  = import_context.current_import_path + f;
        auto res = readF(fp.c_str());
        if (res.size() == 0) return nullptr;
        logger.info("{}", (void*)osize);
        *osize  = (int)res.size();
        auto rs = new char[*osize];
        return (unsigned char*)std::memcpy(rs, res.data(), *osize);
    };
    setupLoaderModule(vm);
    setupTypeBinds(vm);
    setupHelperModule(vm);
    setupEvevntsModule(vm);
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
    current_loader_context.name        = manifest.name;
    import_context.current_import_path = "./plugins/" + manifest.name + "/";
    engine.vm                          = setupVM();
    auto file                          = readF(("./plugins/" + manifest.name + "/" + manifest.entry).c_str());
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
