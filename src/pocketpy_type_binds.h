#pragma once
#include "pocketpy.h"
#include "type_wappers/ActorDamageSource_wapper.h"
#include "type_wappers/actor_wapper.h"


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
    // vm->bind(typeWappersModule,"get")
}
} // namespace engine
} // namespace pocketpy_plugin_loader