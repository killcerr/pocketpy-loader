#pragma once
#include "../pocketpy.h"
#include "Actor_wapper.h"
#include "Item_wapper.h"

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
    static void      _register(VM* vm, PyObject* mod, PyObject* type) {
        vm->bind__eq__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* self, PyObject* other) {
            ItemStackWapper& _self  = _CAST(ItemStackWapper&, self);
            ItemStackWapper& _other = _CAST(ItemStackWapper&, other);
            return VAR((*_self.mItemStack) == (*_other.mItemStack));
        });
        vm->bind_method<0>(type, "getItem", [](VM* vm, ArgsView args) {
            ItemStackWapper& self = _CAST(ItemStackWapper&, args[0]);
            return VAR_T(ItemWapper, ItemWapper(self.mItemStack->mItem.get()));
        });
#define BIND(NAME, ARGC, RTM1, RTM2, EXPR, ...)                                                                        \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        ItemStackWapper& self = _CAST(ItemStackWapper&, args[0]);                                                      \
        EXPR return py_var(vm, RTM1(self.mItemStack->##NAME(__VA_ARGS__)) RTM2);                                       \
    });
#define BIND_VOID(NAME, ARGC, EXPR, ...)                                                                               \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        ItemStackWapper& self = _CAST(ItemStackWapper&, args[0]);                                                      \
        self.mItemStack->##NAME(__VA_ARGS__);                                                                          \
        EXPR return py_var(vm, NoReturn{});                                                                            \
    });
        BIND(toString, 0);
        BIND(getMaxUseDuration, 0);
        BIND(hasItemStackNetId, 0);
        BIND_VOID(releaseUsing, 2, , (Player*)_CAST(ActorWapper&, args[1]).mActor, py_cast<int>(vm, args[2]));
        BIND_VOID(useAsFuel, 0);
        BIND(getTypeName, 0);
        BIND(getDescriptionName, 0);
        BIND(getAuxValue, 0);
        BIND(getCategoryName, 0);
        BIND(getCustomName, 0);
        BIND(getDamageValue, 0);
        BIND(getIdAux, 0);
        BIND(getMaxStackSize, 0);
        BIND(hasUserData, 0);
        BIND_VOID(setStackSize, 1, , py_cast<uchar>(vm, args[1]));
        BIND_VOID(setRepairCost, 1, , py_cast<int>(vm, args[1]));
        BIND_VOID(startCoolDown, 1, , (Player*)_CAST(ActorWapper&, args[1]).mActor);

#undef BIND
    }
};
} // namespace type_wappers
