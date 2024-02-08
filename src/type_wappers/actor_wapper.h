#pragma once
#include <mc/world/actor/Actor.h>

constexpr ::Vec3 make_vec3(float x, float y, float z) { return {x, y, z}; }

#include "../pocketpy.h"


namespace type_wappers {

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
            // auto         pos  = self.mActor->getPosition();
            auto name = _CAST(Str, args[1]);
            self.mActor->setNameTag(name.c_str());
            return py_var(vm, pkpy::NoReturn{});
        });
        vm->bind_method<1>(type, "setPos", [](VM* vm, ArgsView args) {
            ActorWapper& self = _CAST(ActorWapper&, args[0]);
            // auto         pos  = self.mActor->getPosition();
            auto pos = _CAST(::pkpy::Pocketpy_vec3, args[1]);
            self.mActor->setPos({pos.x, pos.y, pos.z});
            return py_var(vm, pkpy::NoReturn{});
        });
    }
    PY_CLASS(ActorWapper, TypeWappers, ActorWapper)
};
} // namespace type_wappers
