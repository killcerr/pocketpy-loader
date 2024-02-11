#pragma once
#include "../pocketpy.h"
#include "BlockPos_wapper.h"
#include "Block_wapper.h"

#include <mc/world/level/BlockSource.h>


namespace type_wappers {
struct BlockSourceWapper {
    BlockSource* mBlockSource;
    BlockSourceWapper() { mBlockSource = nullptr; }
    BlockSourceWapper(BlockSource* p) : mBlockSource(p) {}
    BlockSourceWapper(BlockSourceWapper&& o) { ::std::swap(mBlockSource, o.mBlockSource); }
    BlockSourceWapper(const BlockSourceWapper&) = default;
    ~BlockSourceWapper()                        = default;
    BlockSourceWapper& operator=(const BlockSourceWapper& o) { mBlockSource = o.mBlockSource; }
    BlockSourceWapper& operator=(BlockSourceWapper&& o) { ::std::swap(mBlockSource, o.mBlockSource); }
    PY_CLASS(BlockSourceWapper, TypeWappers, BlockSourceWapper)
    static void _register(VM* vm, PyObject* mod, PyObject* type) {
        vm->bind_func<1>(type, "formAddress", [](VM* vm, ArgsView args) {
            auto s = (BlockSource*)(_CAST(i64, args[0]));
            return VAR_T(BlockSourceWapper, s);
        });
        vm->bind_method<1>(type, "getBlock", [](VM* vm, ArgsView args) {
            BlockSourceWapper& self = _CAST(BlockSourceWapper&, args[0]);
            BlockPos           pos  = _CAST(PyBlockPos&, args[1]);
            return VAR_T(BlockWapper, const_cast<Block*>(&(self.mBlockSource->getBlock(pos))));
        });
        vm->bind_method<2>(type, "setBlock", [](VM* vm, ArgsView args) {
            BlockSourceWapper& self = _CAST(BlockSourceWapper&, args[0]);
            BlockPos           pos  = _CAST(PyBlockPos&, args[1]);
            Block*             blk  = _CAST(BlockWapper, args[2]).mBlock;
            return py_var(vm, self.mBlockSource->setBlockSimple(pos, *blk));
        });
    }
};
} // namespace type_wappers
