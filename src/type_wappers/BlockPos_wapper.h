#pragma once
#include "../pocketpy.h"

#include <mc/world/level/BlockPos.h>


struct PyBlockPos : BlockPos {
    PY_CLASS(PyBlockPos, TypeWappers, BlockPos)
    PyBlockPos() : BlockPos() {}
    PyBlockPos(const BlockPos& v) : BlockPos(v) {}
    PyBlockPos(const PyBlockPos& v) = default;
    PyBlockPos* _() { return this; }
    static void _register(VM* vm, PyObject* mod, PyObject* type) {
        PY_STRUCT_LIKE(PyBlockPos)
        vm->bind_constructor<4>(type, [](VM* vm, ArgsView args) {
            int x = _CAST(int, args[1]);
            int y = _CAST(int, args[2]);
            int z = _CAST(int, args[3]);
            return vm->heap.gcnew<PyBlockPos>(PK_OBJ_GET(Type, args[0]), BlockPos(x, y, z));
        });
        vm->bind__repr__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
            BlockPos self = _CAST(PyBlockPos&, obj);
            SStream  ss;
            ss.setprecision(3);
            ss << "BlockPos(" << self.x << ", " << self.y << ", " << self.z << ")";
            return VAR(ss.str());
        });
        PY_FIELD(PyBlockPos, "x", _, x)
        PY_FIELD(PyBlockPos, "y", _, y)
        PY_FIELD(PyBlockPos, "z", _, z)

#define BIND_BLOCKPOS_OP(POP, OP)                                                                                      \
    vm->bind_method<1>(type, #POP, [](VM* vm, ArgsView args) {                                                         \
        BlockPos& self  = _CAST(PyBlockPos&, args[0]);                                                                 \
        BlockPos& other = _CAST(PyBlockPos&, args[1]);                                                                 \
        return VAR_T(::PyBlockPos, (self OP other));                                                                   \
    });
        BIND_BLOCKPOS_OP(__add__, +);
        BIND_BLOCKPOS_OP(__add__, +)
        BIND_BLOCKPOS_OP(__sub__, -)
        BIND_BLOCKPOS_OP(__mul__, *)
        BIND_BLOCKPOS_OP(__rmul__, *)
        BIND_BLOCKPOS_OP(__truediv__, /)
#undef BIND_BLOCKPOS_OP
        vm->bind__eq__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* self, PyObject* other) {
            BlockPos _self  = _CAST(PyBlockPos&, self);
            BlockPos _other = _CAST(PyBlockPos&, other);
            return VAR(_self == _other);
        });
    }
};

inline pkpy::PyObject* py_var(VM* vm, PyBlockPos pos) { return VAR_T(::PyBlockPos, pos); }