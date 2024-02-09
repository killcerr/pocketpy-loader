#pragma once
#include "../pocketpy.h"

#include <mc/deps/core/string/HashedString.h>
#include <mc/world/item/Item.h>

namespace type_wappers {

struct ItemWapper {
    PY_CLASS(ItemWapper, TypeWappers, ItemWapper)
    Item* mItem;
    ItemWapper() { mItem = nullptr; }
    ItemWapper(Item* p) : mItem(p) {}
    ItemWapper(ItemWapper&& o) { ::std::swap(mItem, o.mItem); }
    ItemWapper(const ItemWapper&) = default;
    ~ItemWapper()                 = default;
    ItemWapper& operator=(const ItemWapper& o) { mItem = o.mItem; }
    ItemWapper& operator=(ItemWapper&& o) { ::std::swap(mItem, o.mItem); }
    static void _register(VM* vm, PyObject* mod, PyObject* type) {
#define BIND(NAME, ARGC, RTM1, RTM2, EXPR, ...)                                                                        \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        ItemWapper& self = _CAST(ItemWapper&, args[0]);                                                                \
        EXPR return py_var(vm, RTM1(self.mItem->##NAME(__VA_ARGS__)) RTM2);                                            \
    });
        BIND(getDescriptionId, 0);
        BIND(isMusicDisk, 0);
        BIND(isArmor, 0);
        BIND(isBlockPlanterItem, 0);
        BIND(isDamageable, 0);
        BIND(isDyeable, 0);
        BIND(isDye, 0);
        BIND(isFood, 0);
        BIND(isThrowable, 0);
        BIND(isUseable, 0);
        BIND(canBeDepleted, 0);
        BIND(isStackedByData, 0);
        BIND(getMaxDamage, 0);
        BIND(getAttackDamage, 0);
        BIND(isHandEquipped, 0);
        BIND(getPatternIndex, 0);
        BIND(canDestroyInCreative, 0);
        BIND(isLiquidClipItem, 0);
        BIND(requiresInteract, 0);
        BIND(getEnchantSlot, 0);
        BIND(getEnchantValue, 0);
        BIND(getArmorValue, 0);
        BIND(getToughnessValue, 0);
        BIND(getViewDamping, 0);
        BIND(isActorPlacerItem, 0);
        BIND(isBucket, 0);
        BIND(isCandle, 0);
        BIND(isComplex, 0);
        BIND(isComponentBased, 0);
        BIND(isFertilizer, 0);
        BIND(isPattern, 0);
        BIND(shouldSendInteractionGameEvents, 0);
        BIND(showsDurabilityInCreative, 0);
        BIND(uniqueAuxValues, 0);
        BIND(useInterruptedByAttacking, 0);
        BIND(allowOffhand, 0);
        BIND(getFrameCount, 0);
        BIND(getFullItemName, 0);
        BIND(getFullNameHash, 0, , .hash);
        BIND(getFurnaceBurnIntervalMultipler, 0);
        BIND(getId, 0);
        // getLegacyBlock
        BIND(getNamespace, 0);
        BIND(shouldDespawn, 0);
#undef BIND
    }
};
} // namespace type_wappers