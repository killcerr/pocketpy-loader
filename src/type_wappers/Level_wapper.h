#pragma once
#include "../pocketpy.h"
#include "Actor_wapper.h"
#include "Dimension_wapper.h"

#include <mc/world/level/Level.h>


namespace type_wappers {
struct LevelWapper {
    Level* mLevel;
    LevelWapper() { mLevel = nullptr; }
    LevelWapper(Level* p) : mLevel(p) {}
    LevelWapper(LevelWapper&& o) { ::std::swap(mLevel, o.mLevel); }
    LevelWapper(const LevelWapper&) = default;
    ~LevelWapper()                  = default;
    LevelWapper& operator=(const LevelWapper& o) { mLevel = o.mLevel; }
    LevelWapper& operator=(LevelWapper&& o) { ::std::swap(mLevel, o.mLevel); }
    PY_CLASS(LevelWapper, TypeWappers, LevelWapper)
    static void _register(VM* vm, PyObject* mod, PyObject* type) {
        vm->bind_func<1>(type, "formAddress", [](VM* vm, ArgsView args) {
            auto s = (Level*)(_CAST(i64, args[0]));
            return VAR_T(LevelWapper, s);
        });
        vm->bind_func<0>(type, "getLevel", [](VM* vm, ArgsView args) {
            return VAR_T(LevelWapper, ll::service::getLevel().as_ptr());
        });
        vm->bind__repr__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
            LevelWapper&  self = PK_OBJ_GET(LevelWapper, obj);
            pkpy::SStream ss;
            ss.write_hex(self.mLevel);
            return VAR(_S("<LevelWapper at ", ss.str(), ">"));
        });
        vm->bind_method<1>(type, "forEachDimension", [](VM* vm, ArgsView args) {
            auto level    = _CAST(LevelWapper&, args[0]).mLevel;
            auto callback = args[1];
            level->forEachDimension([vm, callback](Dimension& dim) {
                return vm->py_bool(vm->call(callback, VAR_T(DimensionWapper, &dim)));
            });
            return VAR(NoReturn{});
        });
        vm->bind_method<1>(type, "forEachPlayer", [](VM* vm, ArgsView args) {
            auto level    = _CAST(LevelWapper&, args[0]).mLevel;
            auto callback = args[1];
            level->forEachPlayer([vm, callback](Player& pl) {
                return vm->py_bool(vm->call(callback, VAR_T(ActorWapper, &pl)));
            });
            return VAR(NoReturn{});
        });
        vm->bind_method<1>(type, "getDimension", [](VM* vm, ArgsView args) {
            auto level = _CAST(LevelWapper&, args[0]).mLevel;
            auto id    = _CAST(int, args[1]);
            return VAR_T(DimensionWapper, level->getDimension(id).get());
        });
    }
};
} // namespace type_wappers
