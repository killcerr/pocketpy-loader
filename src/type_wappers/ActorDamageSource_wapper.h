#pragma once
#include <functional>
#include <magic_enum.hpp>
#include <mc/world/ActorUniqueID.h>
#include <mc/world/actor/ActorDamageSource.h>


// constexpr ::Vec3 make_vec3(float x, float y, float z) { return {x, y, z}; }

#include "../pocketpy.h"

auto ActorDamageCauseToString(auto e) {
#define RT_IF(N)                                                                                                       \
    if (e == ActorDamageCause::##N) return #N
    RT_IF(None);
    RT_IF(Override);
    RT_IF(Contact);
    RT_IF(EntityAttack);
    RT_IF(Projectile);
    RT_IF(Suffocation);
    RT_IF(Fall);
    RT_IF(Fire);
    RT_IF(FireTick);
    RT_IF(Lava);
    RT_IF(Drowning);
    RT_IF(BlockExplosion);
    RT_IF(EntityExplosion);
    RT_IF(Void);
    RT_IF(SelfDestruct);
    RT_IF(Magic);
    RT_IF(Wither);
    RT_IF(Starve);
    RT_IF(Anvil);
    RT_IF(Thorns);
    RT_IF(FallingBlock);
    RT_IF(Piston);
    RT_IF(FlyIntoWall);
    RT_IF(Magma);
    RT_IF(Fireworks);
    RT_IF(Lightning);
    RT_IF(Charging);
    RT_IF(Temperature);
    RT_IF(Freezing);
    RT_IF(Stalactite);
    RT_IF(Stalagmite);
    RT_IF(RamAttack);
    RT_IF(SonicBoom);
    RT_IF(Campfire);
    RT_IF(SoulCampfire);
    RT_IF(All);
#undef RT_IF
    return "";
}

auto StringToActorDamageCause(auto e) {
#define RT_IF(N)                                                                                                       \
    if (e == #N) return ActorDamageCause::##N
    RT_IF(None);
    RT_IF(Override);
    RT_IF(Contact);
    RT_IF(EntityAttack);
    RT_IF(Projectile);
    RT_IF(Suffocation);
    RT_IF(Fall);
    RT_IF(Fire);
    RT_IF(FireTick);
    RT_IF(Lava);
    RT_IF(Drowning);
    RT_IF(BlockExplosion);
    RT_IF(EntityExplosion);
    RT_IF(Void);
    RT_IF(SelfDestruct);
    RT_IF(Magic);
    RT_IF(Wither);
    RT_IF(Starve);
    RT_IF(Anvil);
    RT_IF(Thorns);
    RT_IF(FallingBlock);
    RT_IF(Piston);
    RT_IF(FlyIntoWall);
    RT_IF(Magma);
    RT_IF(Fireworks);
    RT_IF(Lightning);
    RT_IF(Charging);
    RT_IF(Temperature);
    RT_IF(Freezing);
    RT_IF(Stalactite);
    RT_IF(Stalagmite);
    RT_IF(RamAttack);
    RT_IF(SonicBoom);
    RT_IF(Campfire);
    RT_IF(SoulCampfire);
    RT_IF(All);
#undef RT_IF
    return ActorDamageCause::None;
}
auto ActorCategoryToString(auto e) {
#define RT_IF(N)                                                                                                       \
    if (e == ActorCategory::##N) return #N
    RT_IF(None);
    RT_IF(Player);
    RT_IF(Mob);
    RT_IF(Monster);
    RT_IF(Humanoid);
    RT_IF(Animal);
    RT_IF(WaterSpawning);
    RT_IF(Pathable);
    RT_IF(Tamable);
    RT_IF(Ridable);
    RT_IF(UNUSED9);
    RT_IF(Item);
    RT_IF(Ambient);
    RT_IF(Villager);
    RT_IF(Arthropod);
    RT_IF(Undead);
    RT_IF(Zombie);
    RT_IF(Minecart);
    RT_IF(Boat);
    RT_IF(NonTargetable);
    RT_IF(Predictable);
    RT_IF(BoatRideable);
    RT_IF(MinecartRidable);
    RT_IF(HumanoidMonster);
    RT_IF(WaterAnimal);
    RT_IF(TamableAnimal);
    RT_IF(UndeadMob);
    RT_IF(ZombieMonster);
    RT_IF(EvocationIllagerMonster);
#undef RT_IF
    return "";
}

auto StringToActorCategory(auto e) {
#define RT_IF(N)                                                                                                       \
    if (e == #N) return ActorCategory::##N
    RT_IF(None);
    RT_IF(Player);
    RT_IF(Mob);
    RT_IF(Monster);
    RT_IF(Humanoid);
    RT_IF(Animal);
    RT_IF(WaterSpawning);
    RT_IF(Pathable);
    RT_IF(Tamable);
    RT_IF(Ridable);
    RT_IF(UNUSED9);
    RT_IF(Item);
    RT_IF(Ambient);
    RT_IF(Villager);
    RT_IF(Arthropod);
    RT_IF(Undead);
    RT_IF(Zombie);
    RT_IF(Minecart);
    RT_IF(Boat);
    RT_IF(NonTargetable);
    RT_IF(Predictable);
    RT_IF(BoatRideable);
    RT_IF(MinecartRidable);
    RT_IF(HumanoidMonster);
    RT_IF(WaterAnimal);
    RT_IF(TamableAnimal);
    RT_IF(UndeadMob);
    RT_IF(ZombieMonster);
    RT_IF(EvocationIllagerMonster);
#undef RT_IF
    return ActorCategory::None;
}
auto ActorTypeToString(auto e) {
#define RT_IF(N)                                                                                                       \
    if (e == ActorType::##N) return #N
    RT_IF(Undefined);
    RT_IF(TypeMask);
    RT_IF(None);
    RT_IF(Mob);
    RT_IF(PathfinderMob);
    RT_IF(UnknownType);
    RT_IF(Monster);
    RT_IF(Animal);
    RT_IF(WaterAnimal);
    RT_IF(TamableAnimal);
    RT_IF(Ambient);
    RT_IF(UndeadMob);
    RT_IF(ZombieMonster);
    RT_IF(Arthropod);
    RT_IF(Minecart);
    RT_IF(SkeletonMonster);
    RT_IF(EquineAnimal);
    RT_IF(Projectile);
    RT_IF(AbstractArrow);
    RT_IF(VillagerBase);
    RT_IF(Chicken);
    RT_IF(Cow);
    RT_IF(Pig);
    RT_IF(Sheep);
    RT_IF(Wolf);
    RT_IF(Villager);
    RT_IF(MushroomCow);
    RT_IF(Squid);
    RT_IF(Rabbit);
    RT_IF(Bat);
    RT_IF(IronGolem);
    RT_IF(SnowGolem);
    RT_IF(Ocelot);
    RT_IF(Horse);
    RT_IF(Donkey);
    RT_IF(Mule);
    RT_IF(SkeletonHorse);
    RT_IF(ZombieHorse);
    RT_IF(PolarBear);
    RT_IF(Llama);
    RT_IF(Parrot);
    RT_IF(Dolphin);
    RT_IF(Zombie);
    RT_IF(Creeper);
    RT_IF(Skeleton);
    RT_IF(Spider);
    RT_IF(PigZombie);
    RT_IF(Slime);
    RT_IF(EnderMan);
    RT_IF(Silverfish);
    RT_IF(CaveSpider);
    RT_IF(Ghast);
    RT_IF(LavaSlime);
    RT_IF(Blaze);
    RT_IF(ZombieVillager);
    RT_IF(Witch);
    RT_IF(Stray);
    RT_IF(Husk);
    RT_IF(WitherSkeleton);
    RT_IF(Guardian);
    RT_IF(ElderGuardian);
    RT_IF(Npc);
    RT_IF(WitherBoss);
    RT_IF(Dragon);
    RT_IF(Shulker);
    RT_IF(Endermite);
    RT_IF(Agent);
    RT_IF(Vindicator);
    RT_IF(Phantom);
    RT_IF(IllagerBeast);
    RT_IF(UNUSED60);
    RT_IF(ArmorStand);
    RT_IF(TripodCamera);
    RT_IF(Player);
    RT_IF(ItemEntity);
    RT_IF(PrimedTnt);
    RT_IF(FallingBlock);
    RT_IF(MovingBlock);
    RT_IF(ExperiencePotion);
    RT_IF(Experience);
    RT_IF(EyeOfEnder);
    RT_IF(EnderCrystal);
    RT_IF(FireworksRocket);
    RT_IF(Trident);
    RT_IF(Turtle);
    RT_IF(Cat);
    RT_IF(ShulkerBullet);
    RT_IF(FishingHook);
    RT_IF(Chalkboard);
    RT_IF(DragonFireball);
    RT_IF(Arrow);
    RT_IF(Snowball);
    RT_IF(ThrownEgg);
    RT_IF(Painting);
    RT_IF(MinecartRideable);
    RT_IF(LargeFireball);
    RT_IF(ThrownPotion);
    RT_IF(Enderpearl);
    RT_IF(LeashKnot);
    RT_IF(WitherSkull);
    RT_IF(BoatRideable);
    RT_IF(WitherSkullDangerous);
    RT_IF(UNUSED92);
    RT_IF(LightningBolt);
    RT_IF(SmallFireball);
    RT_IF(AreaEffectCloud);
    RT_IF(MinecartHopper);
    RT_IF(MinecartTNT);
    RT_IF(MinecartChest);
    RT_IF(MinecartFurnace);
    RT_IF(MinecartCommandBlock);
    RT_IF(LingeringPotion);
    RT_IF(LlamaSpit);
    RT_IF(EvocationFang);
    RT_IF(EvocationIllager);
    RT_IF(Vex);
    RT_IF(IceBomb);
    RT_IF(Balloon);
    RT_IF(Pufferfish);
    RT_IF(Salmon);
    RT_IF(Drowned);
    RT_IF(Tropicalfish);
    RT_IF(Fish);
    RT_IF(Panda);
    RT_IF(Pillager);
    RT_IF(VillagerV2);
    RT_IF(ZombieVillagerV2);
    RT_IF(Shield);
    RT_IF(WanderingTrader);
    RT_IF(Lectern);
    RT_IF(ElderGuardianGhost);
    RT_IF(Fox);
    RT_IF(Bee);
    RT_IF(Piglin);
    RT_IF(Hoglin);
    RT_IF(Strider);
    RT_IF(Zoglin);
    RT_IF(PiglinBrute);
    RT_IF(Goat);
    RT_IF(GlowSquid);
    RT_IF(Axolotl);
    RT_IF(Warden);
    RT_IF(Frog);
    RT_IF(Tadpole);
    RT_IF(Allay);
    RT_IF(Camel);
    RT_IF(Sniffer);
    RT_IF(TraderLlama);
    RT_IF(ChestBoatRideable);
#undef RT_IF
    return "Unknown";
}
namespace type_wappers {
struct ActorDamageSourceWapper {
    ActorDamageSource* mSource;

    ActorDamageSourceWapper() { mSource = nullptr; }
    ActorDamageSourceWapper(ActorDamageSource* p) : mSource(p) {}
    ActorDamageSourceWapper(ActorDamageSourceWapper&& o) { ::std::swap(mSource, o.mSource); }
    ActorDamageSourceWapper(const ActorDamageSourceWapper&) = default;
    ~ActorDamageSourceWapper()                              = default;
    ActorDamageSourceWapper& operator=(const ActorDamageSourceWapper& o) { mSource = o.mSource; }
    ActorDamageSourceWapper& operator=(ActorDamageSourceWapper&& o) { ::std::swap(mSource, o.mSource); }
    static void              _register(VM* vm, PyObject* mod, PyObject* type) {
        vm->bind__eq__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* lhs, PyObject* rhs) {
            if (!vm->isinstance(rhs, ActorDamageSourceWapper::_type(vm))) return vm->NotImplemented;
            return VAR(
                PK_OBJ_GET(ActorDamageSourceWapper, lhs).mSource == PK_OBJ_GET(ActorDamageSourceWapper, rhs).mSource
            );
        });
        vm->bind__hash__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
            auto& self = PK_OBJ_GET(ActorDamageSourceWapper, obj);
            return (i64)(std::hash<ActorDamageSource*>{}(self.mSource));
        });
        vm->bind__str__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
            ActorDamageSourceWapper& self = PK_OBJ_GET(ActorDamageSourceWapper, obj);
            return VAR(ActorDamageSource::lookupCauseName(self.mSource->getCause()));
        });
        vm->bind__repr__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* obj) {
            ActorDamageSourceWapper& self = PK_OBJ_GET(ActorDamageSourceWapper, obj);
            pkpy::SStream            ss;
            ss.write_hex(self.mSource);
            return VAR(_S("<ActorDamageSourceWapper at ", ss.str(), ">"));
        });
#define BIND(NAME, ARGC, RTM1, RTM2, EXPR, ...)                                                                        \
    vm->bind_method<ARGC>(type, #NAME, [](VM* vm, ArgsView args) {                                                     \
        ActorDamageSourceWapper& self = _CAST(ActorDamageSourceWapper&, args[0]);                                      \
        EXPR return py_var(vm, RTM2(self.mSource->##NAME(__VA_ARGS__) RTM1));                                          \
    })
        BIND(isEntitySource, 0);
        BIND(isBlockSource, 0);
        BIND(isFire, 0);
        BIND(isReducedByResistanceEffect, 0);
        BIND(isReducedByEnchantReduction, 0);
        BIND(isReducedByArmorReduction, 0);
        BIND(isFallingBlockDamage, 0);
        BIND(isFallDamage, 0);
        // getDeathMessage
        BIND(getIsCreative, 0);
        BIND(getIsWorldBuilder, 0);
        BIND(getEntityUniqueID, 0, .id);
        BIND(getEntityType, 0, , ActorTypeToString);
        BIND(getEntityCategories, 0, , ActorCategoryToString);
        BIND(getDamagingEntityIsCreative, 0);
        BIND(getDamagingEntityIsWorldBuilder, 0);
        BIND(getDamagingEntityUniqueID, 0, .id);
        BIND(getDamagingEntityType, 0, , ActorTypeToString);
        BIND(getDamagingEntityCategories, 0, , ActorCategoryToString);
        BIND(clone, 0, .release(), (long long));
        BIND(isChildEntitySource, 0);
        BIND(getCause, 0, , ActorDamageCauseToString);
        vm->bind_method<1>(type, "setCause", [](VM* vm, ArgsView args) {
            auto& self  = _CAST(ActorDamageSourceWapper&, args[0]);
            auto  cause = _CAST(Str&, args[1]);
            self.mSource->setCause(StringToActorDamageCause(cause));
            return py_var(vm, NoReturn{});
        });
#undef BIND
        vm->bind_constructor<1>(type, [](VM* vm, ArgsView args) {
            auto& source = _CAST(Str&, args[1]);
            return vm->heap.gcnew<::type_wappers::ActorDamageSourceWapper>(
                PK_OBJ_GET(Type, args[0]),
                ::type_wappers::ActorDamageSourceWapper(new ActorDamageSource(StringToActorDamageCause(source)))
            );
        });
        vm->bind__eq__(PK_OBJ_GET(Type, type), [](VM* vm, PyObject* self, PyObject* other) {
            ActorDamageSourceWapper& _self  = _CAST(ActorDamageSourceWapper&, self);
            ActorDamageSourceWapper& _other = _CAST(ActorDamageSourceWapper&, other);
            return VAR(_self.mSource->mCause == _other.mSource->mCause);
        });
        vm->bind_func<1>(type, "formAddress", [](VM* vm, ArgsView args) {
            auto s = (ActorDamageSource*)(_CAST(i64, args[0]));
            return VAR_T(ActorDamageSourceWapper, s);
        });
    }
    PY_CLASS(ActorDamageSourceWapper, TypeWappers, ActorDamageSourceWapper)
};
} // namespace type_wappers