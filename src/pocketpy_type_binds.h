#pragma once
#include "pocketpy.h"
#include "type_wappers/actor_wapper.h"

namespace pocketpy_plugin_loader {
namespace engine {
void setupTypeBinds(VM* vm) {
    auto typeWappersModule = vm->new_module("TypeWappers");
    type_wappers::ActorWapper::register_class(vm, typeWappersModule);
}
} // namespace engine
} // namespace pocketpy_plugin_loader