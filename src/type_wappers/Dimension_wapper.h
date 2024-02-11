#pragma once
#include "../pocketpy.h"
#include "BlockPos_wapper.h"
#include "BlockSource_wapper.h"

#include <ll/api/service/Bedrock.h>
#include <mc/world/level/Level.h>
#include <mc/world/level/dimension/Dimension.h>


namespace type_wappers {
struct DimensionWapper {
    Dimension* mDimension;
    DimensionWapper() { mDimension = nullptr; }
    DimensionWapper(Dimension* p) : mDimension(p) {}
    DimensionWapper(DimensionWapper&& o) { ::std::swap(mDimension, o.mDimension); }
    DimensionWapper(const DimensionWapper&) = default;
    ~DimensionWapper()                      = default;
    DimensionWapper& operator=(const DimensionWapper& o) { mDimension = o.mDimension; }
    DimensionWapper& operator=(DimensionWapper&& o) { ::std::swap(mDimension, o.mDimension); }
    PY_CLASS(DimensionWapper, TypeWappers, DimensionWapper)
    static void _register(VM* vm, PyObject* mod, PyObject* type) {
        vm->bind_func<1>(type, "formDimensionId", [](VM* vm, ArgsView args) {
            auto dim = ll::service::getLevel()->getDimension(_CAST(int, args[0])).get();
            return VAR_T(DimensionWapper, dim);
        });
        vm->bind_func<1>(type, "formAddress", [](VM* vm, ArgsView args) {
            auto dim = (Dimension*)(_CAST(i64, args[0]));
            return VAR_T(DimensionWapper, dim);
        });
        vm->bind__repr__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
            DimensionWapper& self = PK_OBJ_GET(DimensionWapper, obj);
            pkpy::SStream    ss;
            ss.write_hex(self.mDimension);
            return VAR(_S("<DimensionWapper at ", ss.str(), ">"));
        });
#define BIND(NAME, ARGC, RTM1, RTM2, EXPR, ...)                                                                        \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        DimensionWapper& self = _CAST(DimensionWapper&, args[0]);                                                      \
        EXPR return py_var(vm, RTM1(self.mDimension->##NAME(__VA_ARGS__)) RTM2);                                       \
    })
#define BIND_VOID(NAME, ARGC, EXPR, ...)                                                                               \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        DimensionWapper& self = _CAST(DimensionWapper&, args[0]);                                                      \
        self.mDimension->##NAME(__VA_ARGS__);                                                                          \
        EXPR return py_var(vm, NoReturn{});                                                                            \
    })
        BIND(getDimensionId, 0);
        BIND(isValidSpawn, 2, , , , _CAST(int, args[1]), _CAST(int, args[2]));
        BIND(isFoggyAt, 2, , , , _CAST(int, args[1]), _CAST(int, args[2]));
        BIND(hasPrecipitationFog, 0);
        BIND(getCloudHeight, 0);
        BIND(getDefaultBiome, 0, , .str);
        BIND(mayRespawnViaBed, 0);
        BIND(hasGround, 0);
        vm->bind_method<0>(type, "getBlockSourceFromMainChunkSource", [](VM* vm, ArgsView args) {
            DimensionWapper& self = _CAST(DimensionWapper&, args[0]);
            return VAR_T(BlockSourceWapper, &(self.mDimension->getBlockSourceFromMainChunkSource()));
        });
#undef BIND
#undef BIND_VOID
    }
};
} // namespace type_wappers
