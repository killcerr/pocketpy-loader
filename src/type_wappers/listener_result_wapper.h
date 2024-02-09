#pragma once
#include "../pocketpy.h"

#include <ll/api/event/EventBus.h>

namespace type_wappers {
struct ListenerResultWapper {
    static void _register(VM* vm, PyObject* mod, PyObject* type) {
        vm->bind__eq__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* lhs, PyObject* rhs) {
            if (!vm->isinstance(rhs, ListenerResultWapper::_type(vm))) return vm->NotImplemented;
            return VAR(
                PK_OBJ_GET(ListenerResultWapper, lhs).listener == PK_OBJ_GET(ListenerResultWapper, rhs).listener
            );
        });
        vm->bind__hash__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
            auto& self = PK_OBJ_GET(ListenerResultWapper, obj);
            return (i64)(std::hash<std::shared_ptr<ll::event::ListenerBase>>{}(self.listener));
        });
        vm->bind__repr__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
            ListenerResultWapper& self = PK_OBJ_GET(ListenerResultWapper, obj);
            pkpy::SStream         ss;
            ss.write_hex(std::addressof(*self.listener));
            return VAR(_S("<ListenerResultWapper at ", ss.str(), ">"));
        });
        vm->bind_method<0>(type, "getId", [](VM* vm, ArgsView args) {
            ListenerResultWapper& self = _CAST(ListenerResultWapper&, args[0]);
            return VAR(self.listener->getId());
        });
        vm->bind_method<0>(type, "removeListener", [](VM* vm, ArgsView args) {
            ListenerResultWapper& self = _CAST(ListenerResultWapper&, args[0]);
            ll::event::EventBus::getInstance().removeListener(self.listener->getId());
            return VAR(pkpy::NoReturn{});
        });
        vm->bind_method<0>(type, "getPriority", [](VM* vm, ArgsView args) {
            ListenerResultWapper& self = _CAST(ListenerResultWapper&, args[0]);
#define RT_IF(P)                                                                                                       \
    if (self.listener->getPriority() == ll::event::EventPriority::##P) return VAR(#P)
            RT_IF(Highest);
            RT_IF(High);
            RT_IF(Normal);
            RT_IF(Low);
            RT_IF(Lowest);
            return VAR("Unknown");
#undef RT_IF
        });
    }
    PY_CLASS(ListenerResultWapper, EventsApi, ListenerResultWapper)
    std::shared_ptr<ll::event::ListenerBase> listener;
    // ll::event::ListenerId id;

    ListenerResultWapper(auto l) { listener = l; }
    ~ListenerResultWapper() {}
};
} // namespace type_wappers