#pragma once
#include <functional>
#include <mc/math/Vec2.h>
#include <mc/world/actor/Actor.h>
#include <mc/world/actor/item/ItemActor.h>
#include <mc/world/actor/player/Player.h>


constexpr ::Vec3 make_vec3(float x, float y, float z) { return {x, y, z}; }
::Pocketpy_vec3  cast_vec3(const Vec3& v) { return {v.x, v.y, v.z}; }
::Pocketpy_vec2  cast_vec2(const Vec2& v) { return {v.x, v.z}; }
::Vec3           cast_vec3(const Pocketpy_vec3& v) { return {v.x, v.y, v.z}; }
::Vec2           cast_vec2(const Pocketpy_vec2& v) { return {v.x, v.y}; }
// extern ActorDamageCause ActorDamageCauseToString(ActorDamageCause);
#include "../pocketpy.h"
#include "ActorDamageSource_wapper.h"
#include "BlockPos_wapper.h"
// #include "ItemStack_wapper.h"


namespace type_wappers {
extern PyObject* _createItemSatckFormActor(VM*, Actor*);
struct ActorWapper {
    Actor* mActor;
    ActorWapper() { mActor = nullptr; }
    ActorWapper(Actor* p) : mActor(p) {}
    ActorWapper(ActorWapper&& o) { ::std::swap(mActor, o.mActor); }
    ActorWapper(const ActorWapper&) = default;
    ~ActorWapper()                  = default;
    ActorWapper& operator=(const ActorWapper& o) { mActor = o.mActor; }
    ActorWapper& operator=(ActorWapper&& o) { ::std::swap(mActor, o.mActor); }
    static void  _register(VM* vm, PyObject* mod, PyObject* type) {
        vm->bind__eq__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* lhs, PyObject* rhs) {
            if (!vm->isinstance(rhs, ActorWapper::_type(vm))) return vm->NotImplemented;
            return VAR(PK_OBJ_GET(ActorWapper, lhs).mActor == PK_OBJ_GET(ActorWapper, rhs).mActor);
        });
        vm->bind__hash__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
            auto& self = PK_OBJ_GET(ActorWapper, obj);
            return (i64)(std::hash<Actor*>{}(self.mActor));
        });
        vm->bind__repr__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
            ActorWapper&  self = PK_OBJ_GET(ActorWapper, obj);
            pkpy::SStream ss;
            ss.write_hex(self.mActor);
            return VAR(_S("<ActorWapper at ", ss.str(), ">"));
        });
        vm->bind_method<0>(type, "getNameTag", [](VM* vm, ArgsView args) {
            ActorWapper& self = _CAST(ActorWapper&, args[0]);
            return py_var(vm, self.mActor->getNameTag());
        });
        vm->bind_method<0>(type, "getPos", [](VM* vm, ArgsView args) {
            ActorWapper& self = _CAST(ActorWapper&, args[0]);
            auto         pos  = self.mActor->getPosition();
            return py_var(vm, pkpy::Pocketpy_vec3{pos.x, pos.y, pos.z});
        });
        vm->bind_method<1>(type, "setNameTag", [](VM* vm, ArgsView args) {
            ActorWapper& self = _CAST(ActorWapper&, args[0]);
            auto         name = _CAST(Str, args[1]);
            self.mActor->setNameTag(name.c_str());
            return py_var(vm, pkpy::NoReturn{});
        });
        vm->bind_method<1>(type, "setPos", [](VM* vm, ArgsView args) {
            ActorWapper& self = _CAST(ActorWapper&, args[0]);
            auto         pos  = _CAST(::pkpy::Pocketpy_vec3, args[1]);
            self.mActor->setPos({pos.x, pos.y, pos.z});
            return py_var(vm, pkpy::NoReturn{});
        });
        vm->bind_method<1>(type, "evalMolang", [](VM* vm, ArgsView args) {
            ActorWapper& self = _CAST(ActorWapper&, args[0]);
            auto         expr = _CAST(Str&, args[1]);
            return py_var(vm, self.mActor->evalMolang(expr.c_str()));
        });
        vm->bind_method<1>(type, "addTag", [](VM* vm, ArgsView args) {
            ActorWapper& self = _CAST(ActorWapper&, args[0]);
            auto         tag  = _CAST(Str&, args[1]);
            return py_var(vm, self.mActor->addTag(tag.c_str()));
        });
#define BIND(NAME, ARGC, RTM1, RTM2, EXPR, ...)                                                                        \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        ActorWapper& self = _CAST(ActorWapper&, args[0]);                                                              \
        EXPR return py_var(vm, RTM1(self.mActor->##NAME(__VA_ARGS__)) RTM2);                                           \
    })
#define BIND_VOID(NAME, ARGC, EXPR, ...)                                                                               \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        ActorWapper& self = _CAST(ActorWapper&, args[0]);                                                              \
        self.mActor->##NAME(__VA_ARGS__);                                                                              \
        EXPR return py_var(vm, NoReturn{});                                                                            \
    })
        BIND(getEntityContext, 0, (void*)&);
        BIND(getTypeName, 0);
        BIND(getFeetPos, 0, cast_vec3);
        BIND(getHeadPos, 0, cast_vec3);
        BIND(getFeetBlockPos, 0, PyBlockPos);
        BIND(isSimulatedPlayer, 0);
        BIND(isOnGround, 0);
        BIND_VOID(setOnFire, 2, , _CAST(int, args[1]), _CAST(bool, args[2]));
        BIND_VOID(stopFire, 0);
        BIND(getPosDeltaPerSecLength, 0);
        BIND(
            hurtByCause,
            3,
            ,
            ,
            ,
            _CAST(float, args[1]),
            StringToActorDamageCause(_CAST(Str&, args[2])),
            _CAST(ActorWapper&, args[3]).mActor
        );
        BIND_VOID(
            teleport,
            3,
            ,
            cast_vec3(_CAST(Pocketpy_vec3, args[1])),
            _CAST(int, args[2]),
            cast_vec2(_CAST(Pocketpy_vec2, args[3]))
        );
        BIND_VOID(setName, 1, , _CAST(Str&, args[1]).c_str());
        BIND_VOID(kill, 0);
        BIND_VOID(despawn, 0);
        BIND_VOID(die, 1, , *(_CAST(::type_wappers::ActorDamageSourceWapper&, args[1]).mSource));
        // BIND(isPlayer, 0);
        // BIND(isLocalPlayer, 0);
        BIND(isFireImmune, 0);
        BIND_VOID(
            playerTouch,
            1,
            if (_CAST(::type_wappers::ActorWapper&, args[1]).mActor->getTypeName() == "minecraft:player") {
                vm->RuntimeError("args[1] is not a player");
                return py_var(vm, NoReturn{});
            },
            *(Player*)(_CAST(::type_wappers::ActorWapper&, args[1]).mActor)
        );
        BIND(isCreative, 0);
#define PLBIND(NAME, ARGC, RTM1, RTM2, EXPR, ...)                                                                      \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        ActorWapper& self = _CAST(ActorWapper&, args[0]);                                                              \
        if (!(self.mActor->getTypeName() == "minecraft:player")) {                                                     \
            vm->RuntimeError("self is not a player");                                                                  \
            return py_var(vm, NoReturn{});                                                                             \
        }                                                                                                              \
        EXPR return py_var(vm, RTM1(((Player*)(self.mActor))->##NAME(__VA_ARGS__)) RTM2);                              \
    })
#define PLBIND_VOID(NAME, ARGC, EXPR, ...)                                                                             \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        ActorWapper& self = _CAST(ActorWapper&, args[0]);                                                              \
        if (!(self.mActor->getTypeName() == "minecraft:player")) {                                                     \
            vm->RuntimeError("self is not a player");                                                                  \
            return py_var(vm, NoReturn{});                                                                             \
        }                                                                                                              \
        ((Player*)(self.mActor))->##NAME(__VA_ARGS__);                                                                 \
        EXPR return py_var(vm, NoReturn{});                                                                            \
    })
        PLBIND(getIPAndPort, 0);
        PLBIND(isOperator, 0);
        PLBIND(getRealName, 0);
        PLBIND(getLocaleName, 0);
        PLBIND_VOID(disconnect, 1, , _CAST(Str&, args[1]).sv());
        PLBIND_VOID(sendMessage, 1, , _CAST(Str&, args[1]).sv());
#define IABIND(NAME, ARGC, RTM1, RTM2, EXPR, ...)                                                                      \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        ActorWapper& self = _CAST(ActorWapper&, args[0]);                                                              \
        if (!(self.mActor->getTypeName() == "minecraft:item")) {                                                       \
            vm->RuntimeError("self is not a item");                                                                    \
            return py_var(vm, NoReturn{});                                                                             \
        }                                                                                                              \
        EXPR return py_var(vm, RTM1(((ItemActor*)(self.mActor))->##NAME(__VA_ARGS__)) RTM2);                           \
    })
#define IABIND_VOID(NAME, ARGC, EXPR, ...)                                                                             \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        ActorWapper& self = _CAST(ActorWapper&, args[0]);                                                              \
        if (!(self.mActor->getTypeName() == "minecraft:item")) {                                                       \
            vm->RuntimeError("self is not a item");                                                                    \
            return py_var(vm, NoReturn{});                                                                             \
        }                                                                                                              \
        ((ItemActor*)(self.mActor))->##NAME(__VA_ARGS__);                                                              \
        EXPR return py_var(vm, NoReturn{});                                                                            \
    })
        vm->bind_method<0>(type, "item", [](VM* vm, ArgsView args) {
            ActorWapper& self = _CAST(ActorWapper&, args[0]);
            if (!(self.mActor->getTypeName() == "minecraft:item")) {
                vm->RuntimeError("self is not a item");
                return py_var(vm, NoReturn{});
            }
            return _createItemSatckFormActor(vm, self.mActor);
        });
        IABIND(age, 0);
        IABIND(pickupDelay, 0);
        IABIND(throwTime, 0);
        IABIND(bobOffs, 0);
        IABIND(health, 0);
        IABIND(lifeTime, 0);
        IABIND(isInItemFrame, 0);
        IABIND(isFromFishing, 0);
        IABIND_VOID(setSourceEntity, 1, , _CAST(ActorWapper&, args[0]).mActor);
#undef BIND
#undef PLBIND
#undef IABIND
#undef BIND_VOID
#undef PLBIND_VOID
#undef IABIND_VOID
        vm->bind_func<1>(type, "formAddress", [](VM* vm, ArgsView args) {
            auto ac = (Actor*)(_CAST(i64, args[0]));
            return VAR_T(ActorWapper, ac);
        });
    }
    PY_CLASS(ActorWapper, TypeWappers, ActorWapper)
};
} // namespace type_wappers