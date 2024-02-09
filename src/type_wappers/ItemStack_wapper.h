#pragma once
#include "../pocketpy.h"
#include "Actor_wapper.h"

#include <mc/world/item/registry/ItemStack.h>

namespace type_wappers {

struct ItemStackWapper {
    PY_CLASS(ItemStackWapper, TypeWappers, ItemStackWapper)
    ItemStack* mItemStack;
    ItemStackWapper() { mItemStack = nullptr; }
    ItemStackWapper(ItemStack* p) : mItemStack(p) {}
    ItemStackWapper(ItemStackWapper&& o) { ::std::swap(mItemStack, o.mItemStack); }
    ItemStackWapper(const ItemStackWapper&) = default;
    ~ItemStackWapper()                      = default;
    ItemStackWapper& operator=(const ItemStackWapper& o) { mItemStack = o.mItemStack; }
    ItemStackWapper& operator=(ItemStackWapper&& o) { ::std::swap(mItemStack, o.mItemStack); }
    static void      _register(VM* vm, PyObject* mod, PyObject* type) {}
};
} // namespace type_wappers
PyObject* py_var(VM* vm, const type_wappers::ItemStackWapper& is) { return VAR_T(type_wappers::ItemStackWapper, is); }