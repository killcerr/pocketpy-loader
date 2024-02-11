#pragma once
#include "../pocketpy.h"
#include "Actor_wapper.h"

#include <mc/common/Brightness.h>
#include <mc/deps/core/string/HashedString.h>
// #include <mc/world/actor/item/ItemActor.h>
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
        vm->bind__repr__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
            ItemWapper&   self = PK_OBJ_GET(ItemWapper, obj);
            pkpy::SStream ss;
            ss.write_hex(self.mItem);
            return VAR(_S("<ItemWapper at ", ss.str(), ">"));
        });
#define BIND(NAME, ARGC, RTM1, RTM2, EXPR, ...)                                                                        \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        ItemWapper& self = _CAST(ItemWapper&, args[0]);                                                                \
        EXPR return py_var(vm, RTM1(self.mItem->##NAME(__VA_ARGS__)) RTM2);                                            \
    })
#define BIND_VOID(NAME, ARGC, EXPR, ...)                                                                               \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        ItemWapper& self = _CAST(ItemWapper&, args[0]);                                                                \
        self.mItem->##NAME(__VA_ARGS__);                                                                               \
        EXPR return py_var(vm, NoReturn{});                                                                            \
    })
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

        BIND_VOID(setMaxDamage, 1, , _CAST(int, args[1]));
        BIND_VOID(setMaxUseDuration, 1, , _CAST(int, args[1]));

        BIND(isValidAuxValue, 1, , , , _CAST(int, args[1]));
        BIND(getDamageChance, 1, , , , _CAST(int, args[1]));
        BIND(validFishInteraction, 1, , , , _CAST(int, args[1]));
        // BIND(
        //     getInteractText,
        //     1,
        //     ,
        //     ,
        //     {
        //         if (_CAST(::type_wappers::ActorWapper&, args[1]).mActor->getTypeName() == "minecraft:player") {
        //             vm->RuntimeError("args[1] is not a player");
        //             return py_var(vm, NoReturn{});
        //         }
        //     },
        //     *(Player*)(_CAST(::type_wappers::ActorWapper&, args[1]).mActor)
        // );
        BIND(isEmissive, 1, , , , _CAST(int, args[1]));
        BIND(getLightEmission, 1, , .value, , _CAST(int, args[1]));
        BIND(getIconYOffset, 0);
        BIND(getAuxValuesDescription, 0);
        BIND(isElytra, 0);
        BIND(isExplodable, 0);
        BIND(isFireResistant, 0);
        BIND(isNameTag, 0);
        BIND(isSeed, 0);
        BIND_VOID(setAllowOffhand, 1, , _CAST(bool, args[1]));
        BIND_VOID(setExplodable, 1, , _CAST(bool, args[1]));
        BIND_VOID(setFireResistant, 1, , _CAST(bool, args[1]));
        BIND_VOID(setFurnaceBurnIntervalMultiplier, 1, , _CAST(float, args[1]));
        BIND_VOID(setFurnaceXPmultiplier, 1, , _CAST(float, args[1]));
        BIND_VOID(setHandEquipped, 0);
        BIND_VOID(setIsGlint, 1, , _CAST(bool, args[1]));
        BIND_VOID(setIsMirroredArt, 1, , _CAST(bool, args[1]));
        BIND_VOID(setMaxStackSize, 1, , _CAST(uchar, args[1]));
        BIND_VOID(setRequiresWorldBuilder, 1, , _CAST(bool, args[1]));
        BIND_VOID(setShouldDespawn, 1, , _CAST(bool, args[1]));
        BIND_VOID(setStackedByData, 1, , _CAST(bool, args[1]));

#undef BIND
#undef BIND_VOID
        vm->bind_func<1>(type, "formAddress", [](VM* vm, ArgsView args) {
            auto s = (Item*)(_CAST(i64, args[0]));
            return VAR_T(ItemWapper, s);
        });
    }
};
} // namespace type_wappers