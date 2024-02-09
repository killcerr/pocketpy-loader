#pragma once
#include "pocketpy.h"
#include "type_wappers/ActorDamageSource_wapper.h"
#include "type_wappers/Actor_wapper.h"
#include "type_wappers/BlockPos_wapper.h"
#include "type_wappers/ItemStack_wapper.h"
#include "type_wappers/Item_wapper.h"
#include "type_wappers/listener_result_wapper.h"


namespace pocketpy_plugin_loader {
namespace engine {
void setupTypeBinds(VM* vm) {
    auto typeWappersModule = vm->new_module("TypeWappers");
    type_wappers::ActorWapper::register_class(vm, typeWappersModule);
    vm->bind(typeWappersModule, "getActorFromAddress(address:int)->ActorWapper", [](VM* vm, ArgsView args) {
        auto                      address = _CAST(long long, args[0]);
        type_wappers::ActorWapper wapper((Actor*)address);
        return VAR_T(type_wappers::ActorWapper, wapper);
    });
    type_wappers::ActorDamageSourceWapper::register_class(vm, typeWappersModule);
    vm->bind(
        typeWappersModule,
        "getActorDamageSourceFromAddress(address:int)->ActorDamageSourceWapper",
        [](VM* vm, ArgsView args) {
            auto                                  address = _CAST(long long, args[0]);
            type_wappers::ActorDamageSourceWapper wapper((ActorDamageSource*)address);
            return VAR_T(type_wappers::ActorDamageSourceWapper, wapper);
        }
    );
    PyBlockPos::register_class(vm, typeWappersModule);
    vm->bind(typeWappersModule, "getBlockPosFormAddress(address:int)->PyBlockPos", [](VM* vm, ArgsView args) {
        auto address = _CAST(long long, args[0]);
        return VAR_T(PyBlockPos, *((PyBlockPos*)address));
    });
    type_wappers::ItemWapper::register_class(vm, typeWappersModule);
    vm->bind(typeWappersModule, "getItemFormAddress(address:int)->PyBlockPos", [](VM* vm, ArgsView args) {
        auto  address = _CAST(long long, args[0]);
        Item* item    = (Item*)address;
        return VAR_T(type_wappers::ItemWapper, type_wappers::ItemWapper(item));
    });
    type_wappers::ItemStackWapper::register_class(vm, typeWappersModule);
    vm->bind(typeWappersModule, "getItemStackFormAddress(address:int)->PyBlockPos", [](VM* vm, ArgsView args) {
        auto       address = _CAST(long long, args[0]);
        ItemStack* item    = (ItemStack*)address;
        return VAR_T(type_wappers::ItemStackWapper, type_wappers::ItemStackWapper(item));
    });
    type_wappers::ListenerResultWapper::register_class(vm, typeWappersModule);

    // vm->bind(typeWappersModule,"get")
}
} // namespace engine
} // namespace pocketpy_plugin_loader