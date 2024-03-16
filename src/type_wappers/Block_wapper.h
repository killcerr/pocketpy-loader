#pragma once
#include "../pocketpy.h"

#include <mc/common/Brightness.h>
#include <mc/nbt/CompoundTag.h>
#include <mc/world/level/block/Block.h>


namespace type_wappers {
struct BlockWapper {
    Block* mBlock;
    BlockWapper() { mBlock = nullptr; }
    BlockWapper(Block* p) : mBlock(p) {}
    BlockWapper(BlockWapper&& o) { ::std::swap(mBlock, o.mBlock); }
    BlockWapper(const BlockWapper&) = default;
    ~BlockWapper()                  = default;
    BlockWapper& operator=(const BlockWapper& o) { mBlock = o.mBlock; }
    BlockWapper& operator=(BlockWapper&& o) { ::std::swap(mBlock, o.mBlock); }
    PY_CLASS(BlockWapper, TypeWappers, BlockWapper)
    static void _register(VM* vm, PyObject* mod, PyObject* type) {
        vm->bind_func<1>(type, "formAddress", [](VM* vm, ArgsView args) {
            auto blk = (Block*)(_CAST(i64, args[0]));
            return VAR_T(BlockWapper, blk);
        });
        vm->bind__repr__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
            BlockWapper&  self = PK_OBJ_GET(BlockWapper, obj);
            pkpy::SStream ss;
            ss.write_hex(self.mBlock);
            return VAR(_S("<BlockWapper at ", ss.str(), ">"));
        });
        vm->bind_func<1>(type, "formRuntimeID", [](VM* vm, ArgsView args) {
            return VAR_T(BlockWapper, (Block*)Block::tryGetFromRegistry(_CAST(uint, args[0])).as_ptr());
        });
        vm->bind_func<1>(type, "formName", [](VM* vm, ArgsView args) {
            return VAR_T(BlockWapper, (Block*)Block::tryGetFromRegistry(_CAST(Str&, args[0]).sv()).as_ptr());
        });
        vm->bind_func<2>(type, "formNameAndData", [](VM* vm, ArgsView args) {
            return VAR_T(
                BlockWapper,
                (Block*)Block::tryGetFromRegistry(_CAST(Str&, args[0]).sv(), _CAST(ushort, args[1])).as_ptr()
            );
        });
        vm->bind_func<1>(type, "formSnbt", [](VM* vm, ArgsView args) {
            auto nbt = CompoundTag::fromSnbt(_CAST(Str&, args[0]).sv());
            return VAR_T(BlockWapper, (Block*)Block::tryGetFromRegistry(*nbt).as_ptr());
        });
#define BIND(NAME, ARGC, RTM1, RTM2, EXPR, ...)                                                                        \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        BlockWapper& self = _CAST(BlockWapper&, args[0]);                                                              \
        EXPR return py_var(vm, RTM1(self.mBlock->##NAME(__VA_ARGS__)) RTM2);                                           \
    })
#define BIND_VOID(NAME, ARGC, EXPR, ...)                                                                               \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        BlockWapper& self = _CAST(BlockWapper&, args[0]);                                                              \
        self.mBlock->##NAME(__VA_ARGS__);                                                                              \
        EXPR return py_var(vm, NoReturn{});                                                                            \
    })
        BIND(getTypeName, 0);
        BIND(getData, 0);
        BIND(getBlockItemId, 0);
        BIND(canBeOriginalSurface, 0);
        // BIND(canContainLiquid, 0);
        BIND(canDamperVibrations, 0);
        BIND(canHaveExtraData, 0);
        BIND(canHurtAndBreakItem, 0);
        BIND(canOccludeVibrations, 0);
        BIND(canProvideFullSupport, 1, , , , _CAST(uchar, args[1]));
        BIND(canProvideMultifaceSupport, 1, , , , _CAST(uchar, args[1]));
        BIND(canReactToNeighborsDuringInstatick, 0);
        BIND(causesFreezeEffect, 0);
        BIND(getDescriptionId, 0);
        BIND(getDestroySpeed, 0);
        BIND(getFriction, 0);
        BIND(getLight, 0, , .value);
        BIND(getLightEmission, 0, , .value);
        BIND(isAir, 0);
        BIND(isBounceBlock, 0);
        BIND(isButtonBlock, 0);
        BIND(isContainerBlock, 0);
        BIND(isCropBlock, 0);
        BIND(isDoorBlock, 0);
        BIND(isEmpty, 0);
        BIND(isFallingBlock, 0);
        BIND(isFenceBlock, 0);
        BIND(isFenceGateBlock, 0);
        BIND(isInteractiveBlock, 0);
        BIND(isLavaFlammable, 0);
        BIND(isLeverBlock, 0);
        BIND(isMotionBlockingBlock, 0);
        BIND(isMultifaceBlock, 0);
        BIND(isRailBlock, 0);
    }
#undef BIND
#undef BIND_VOID
};
} // namespace type_wappers
