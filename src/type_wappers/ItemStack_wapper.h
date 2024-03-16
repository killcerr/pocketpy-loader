#pragma once
#include "../pocketpy.h"
// #include "Actor_wapper.h"
#include "Item_wapper.h"

#include <mc/common/wrapper/InteractionResult.h>
#include <mc/world/item/registry/ItemStack.h>


namespace type_wappers {
struct ActorWapper;
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
        vm->bind__repr__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
            ItemStackWapper& self = PK_OBJ_GET(ItemStackWapper, obj);
            pkpy::SStream    ss;
            ss.write_hex(self.mItemStack);
            return VAR(_S("<ItemStackWapper at ", ss.str(), ">"));
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
        EXPR             self.mItemStack->##NAME(__VA_ARGS__);                                                         \
        return py_var(vm, NoReturn{});                                                                                 \
    });
        BIND(toString, 0);
        BIND(getMaxUseDuration, 0);
        BIND(hasItemStackNetId, 0);
        BIND_VOID(releaseUsing, 2, , ((Player*)(_CAST(ActorWapper&, args[1]).mActor)), (py_cast<int>(vm, args[2])));
        BIND_VOID(useAsFuel, 0);
        BIND(getTypeName, 0);
        BIND(getDescriptionName, 0);
        BIND(getAuxValue, 0);
        // BIND(getCategoryName, 0);
        BIND(getCustomName, 0);
        BIND(getDamageValue, 0);
        BIND(getIdAux, 0);
        BIND(getMaxStackSize, 0);
        BIND(hasUserData, 0);
        // BIND_VOID(setStackSize, 1, , py_cast<uchar>(vm, args[1]));
        BIND_VOID(setRepairCost, 1, , py_cast<int>(vm, args[1]));
        BIND_VOID(startCoolDown, 1, , (Player*)_CAST(ActorWapper&, args[1]).mActor);
        vm->bind_method<1>(type, "use", [](VM* vm, ArgsView args) {
            ItemStackWapper& self = _CAST(ItemStackWapper&, args[0]);
            return VAR_T(
                ItemStackWapper,
                ItemStackWapper(&(self.mItemStack->use(*(Player*)_CAST(ActorWapper&, args[1]).mActor)))
            );
        });
        // clang-format off
        BIND_VOID(useOn, 6, auto vec = cast_vec3(_CAST(Pocketpy_vec3, args[6]));, *(_CAST(ActorWapper&, args[1]).mActor), _CAST(int, args[2]), _CAST(int, args[3]), _CAST(int, args[4]), _CAST(uchar, args[5]), vec);
        // clang-format on
        vm->bind_func<1>(type, "formAddress", [](VM* vm, ArgsView args) {
            auto s = (ItemStack*)(_CAST(i64, args[0]));
            return VAR_T(ItemStackWapper, s);
        });
#undef BIND
    }
};
PyObject* _createItemSatckFormActor(VM* vm, Actor* actor) {
    auto& is = ((ItemActor*)actor)->item();
    return VAR_T(ItemStackWapper, &is);
}
} // namespace type_wappers