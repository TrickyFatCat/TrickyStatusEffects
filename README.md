# About

**TrickyStatusEffects** provides a flexible and comprehensive status effect system for Unreal Engine 5 projects.

## Features

* **Flexible Status Effect System**: Create and script custom status effects with build in tick, timer, and stacks
* **Status Effect Types**: Support for Positive, Negative, and Neutral status effect categorization
* **Blueprint-Friendly**: Easy-to-use Blueprint functions for all status effect operations

## Installation

1. Copy the plugin folder into your project's `Plugins` directory.
2. Open your Unreal Engine project.
3. Enable the **TrickyStatusEffects** in the plugin menu.
4. Restart the editor.

## Basic Setup

1. Add `UStatusEffectsManagerComponent` to any actor that should be able to receive status effects.
2. Create custom status effects by inheriting from `UStatusEffectBase`. 
3. Implement all necessary logic by overriding functions in Blueprints or C++:
   1. `CanBeActivated()`
   2. `ActivateEffect()`
   3. `TickEffect(float DeltaTime)`
   4. `DeactivateEffect(AActor* Deactivator)`
   5. `RefreshEffect()`
   6. `IncreaseStacks(int32 Amount)`
   7. `DecreaseStacks(int32 Amount)`
4. Configure status effect properties (duration, stacks, type, etc.).
5. Apply status effects using functions from `UStatusEffectsLibrary` or directly from the manager component.

## Functions

### StatusEffectsManagerComponent
The `UStatusEffectsManagerComponent` is an Actor Component responsible for managing all status effects applied to its owner.

**Key Functions:**
* `ApplyStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Instigator)`: Applies a status effect to the owner.
* `RefreshAllStatusEffects()`: Refreshes all currently active status effects.
* `RemoveStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Remover)`: Removes a specific status effect.
* `RemoveAllStatusEffects(AActor* Remover)`: Removes all active status effects.
* `HasStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect)`: Checks if a specific status effect is active.
* `GetStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect)`: Gets a reference to an active status effect.
* `GetAllStatusEffects(TArray<UStatusEffectBase*>& OutStatusEffects)`: Retrieves all currently active status effects.

**Filtering Functions:**
* Functions with `FromInstigator` suffix allow filtering by the actor who applied the effect
* Functions with `OfClass` suffix allow filtering by specific status effect classes
* Functions with `OfType` suffix allow filtering by status effect type (Positive, Negative, Neutral)

**Delegates:**
* `OnStatusEffectApplied`: Called when a status effect is successfully applied.
* `OnStatusEffectRemoved`: Called when a status effect is removed.
* `OnStatusEffectRefreshed`: Called when a status effect is refreshed.

### StatusEffectBase
The `UStatusEffectBase` class is the foundation for all status effects in the system.

**Key Properties:**
* `EffectType (EStatusEffectType)`: Categorizes the effect as Positive, Negative, or Neutral.
* `EffectScope (EStatusEffectScope)`: Determines if the effect is per-target or global.
* `Duration (float)`: How long the effect lasts (if not infinite).
* `bIsInfinite (bool)`: Whether the effect lasts indefinitely.
* `bIsStackable (bool)`: Whether multiple instances can stack.
* `MaxStacks (int32)`: Maximum number of stacks allowed.
* `TimerBehavior (EStatusEffectTimerRefreshBehavior)`: How the timer behaves when refreshed.
* `StacksBehavior (EStatusEffectStacksRefreshBehavior)`: How stacks behave when refreshed.

**Key Functions:**
* `CanBeActivated()`: Blueprint event to determine if the effect can be applied.
* `ActivateEffect()`: Blueprint event called when the effect is first applied.
* `TickEffect(float DeltaTime)`: Blueprint event called every tick while active.
* `DeactivateEffect(AActor* Deactivator)`: Blueprint event called when the effect is removed.
* `RefreshEffect()`: Blueprint event called when the effect is refreshed.
* `IncreaseStacks(int32 Amount)`: Manually increase the stack count.
* `DecreaseStacks(int32 Amount)`: Manually decrease the stack count.

**Delegates:**
* `OnStatusEffectDeactivated`: Called when the effect is deactivated.
* `OnStatusEffectRefreshed`: Called when the effect is refreshed.
* `OnStatusEffectStacksIncreased`: Called when stacks are increased.
* `OnStatusEffectStacksDecreased`: Called when stacks are decreased.

### StatusEffectsLibrary
`UStatusEffectsLibrary` provides static Blueprint utility functions for the status effect system.

**Key Functions:**
* `GetStatusEffectsManager(AActor* Target)`: Gets the status effects manager component from an actor.
* `ApplyStatusEffect(AActor* Target, TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Instigator)`: Applies a status effect to a target actor.
* `HasStatusEffect(AActor* Target, TSubclassOf<UStatusEffectBase> StatusEffect)`: Checks if a target has a specific status effect.
* `RemoveStatusEffect(AActor* Target, TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Remover)`: Removes a status effect from a target.

All functions support the same filtering options as the manager component (by instigator, class, and type).