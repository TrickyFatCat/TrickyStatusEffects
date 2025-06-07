// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "StatusEffectBase.h"
#include "Components/ActorComponent.h"
#include "StatusEffectsManagerComponent.generated.h"

class UStatusEffectBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStatusEffectAppliedDynamicSignature,
                                               UStatusEffectsManagerComponent*, Component,
                                               UStatusEffectBase*, StatusEffect,
                                               AActor*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStatusEffectRemovedDynamicSignature,
                                               UStatusEffectsManagerComponent*, Component,
                                               UStatusEffectBase*, StatusEffect,
                                               AActor*, Remover);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusEffectRefreshedDynamicSignature,
                                             UStatusEffectsManagerComponent*, Component,
                                             UStatusEffectBase*, StatusEffect);

/**
 * Responsible for managing status effects applied to an actor.
 */
UCLASS(ClassGroup=(TrickyStatusEffects), meta=(BlueprintSpawnableComponent))
class TRICKYSTATUSEFFECTS_API UStatusEffectsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatusEffectsManagerComponent();

	/**
	 * Called when a new status effect was applied
	 */
	UPROPERTY(BlueprintAssignable, Category="StatusEffects")
	FOnStatusEffectAppliedDynamicSignature OnStatusEffectApplied;

	/**
	 * Called when a status effect was removed
	 */
	UPROPERTY(BlueprintAssignable, Category="StatusEffects")
	FOnStatusEffectRemovedDynamicSignature OnStatusEffectRemoved;

	/**
	 * Called when a status effect was refreshed
	 */
	UPROPERTY(BlueprintAssignable, Category="StatusEffects")
	FOnStatusEffectRefreshedDynamicSignature OnStatusEffectRefreshed;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	/**
	 * Applies a status effect
	 * If the effect is already applied and its scope is not PerInstance, it will refresh it
	 *
	 * @param StatusEffect The class of the status effect to be applied
	 * @param Instigator The actor which applied a status effect. Can be nullptr
	 * @return A pointer to the applied status effect object. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	UStatusEffectBase* ApplyStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Instigator);

	/**
	 * Refreshes all applied status effects
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffects();

	/**
	 * Refreshes all applied status effects of the specified class
	 *
	 * @param StatusEffect The class of the status effects to be refreshed
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsOfClass(TSubclassOf<UStatusEffectBase> StatusEffect) const;

	/**
	 * Refreshes all status effects applied by a specified instigator
	 *
	 * @param Instigator The actor which applied status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsFromInstigator(AActor* Instigator) const;

	/**
	 * Refreshes all active status effects of a specified class that were applied by a specified instigator
	 *
	 * @param StatusEffect The class of the status effects to be refreshed
	 * @param Instigator The actor which applied status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsOfClassFromInstigator(TSubclassOf<UStatusEffectBase> StatusEffect,
	                                                  AActor* Instigator) const;

	/**
	 * Refreshes all applied status effects of the specific type
	 *
	 * @param StatusEffectType The type of status effects to refresh
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsOfType(const EStatusEffectType StatusEffectType) const;

	/**
	 * Refreshes all active status effects of the specific type that were applied by a specified instigator
	 *
	 * @param StatusEffectType The type of status effects to refresh.
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsOfTypeFromInstigator(const EStatusEffectType StatusEffectType,
	                                                 AActor* Instigator) const;

	/**
	 * Removes a specified status effect from applied status effects.
	 *
	 * @param StatusEffect The class of the status effect to remove
	 * @param Remover The actor attempting to remove the status effect. Can be nullptr
	 * @return True if the status effect was successfully removed
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool RemoveStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Remover);

	/**
	 * Removes a specified status effect applied by a specified instigator.
	 *
	 * @param StatusEffect The class of the status effect to remove
	 * @param Instigator The actor which applied the status effect. Can be nullptr
	 * @param Remover The actor attempting to remove the status effect. Can be nullptr
	 * @return True if the status effect was successfully removed
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool RemoveStatusEffectFromInstigator(TSubclassOf<UStatusEffectBase> StatusEffect,
	                                      AActor* Instigator,
	                                      AActor* Remover);

	/**
	 * Removes all currently applied status effects
	 *
	 * @param Remover The actor responsible for removing the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffects(AActor* Remover);

	/**
	 * Removes all status effects applied by a specified instigator
	 *
	 * @param Instigator The actor that originally applied the status effects. Can be nullptr
	 * @param Remover The actor responsible for removing the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsFromInstigator(AActor* Instigator, AActor* Remover) const;

	/**
	 * Removes all applied status effects of the specified class
	 *
	 * @param StatusEffect The class of the status effects to remove
	 * @param Remover The actor responsible for removing the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsOfClass(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Remover) const;

	/**
	 * Removes all applied status effects of the specified class applied by a specified instigator
	 *
	 * @param StatusEffect The class of the status effects to remove
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 * @param Remover The actor responsible for removing the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsOfClassFromInstigator(TSubclassOf<UStatusEffectBase> StatusEffect,
	                                                 AActor* Instigator,
	                                                 AActor* Remover) const;

	/**
	 * Removes all applied status effects of a specific type
	 *
	 * @param StatusEffectType The type of status effects to remove
	 * @param Remover The actor responsible for removing the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsOfType(const EStatusEffectType StatusEffectType, AActor* Remover) const;

	/**
	 * Removes all applied status effects of a specific type applied by a specified instigator
	 *
	 * @param StatusEffectType The type of status effect to remove
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 * @param Remover The actor responsible for removing the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsOfTypeFromInstigator(const EStatusEffectType StatusEffectType,
	                                                AActor* Instigator,
	                                                AActor* Remover) const;

	/**
	 * Checks if a specified status effect is currently applied to the actor
	 *
	 * @param StatusEffect The class of the status effect to check for
	 * @return True if the status effect is applied
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect) const;

	/**
	 * Checks if a specified status effect from a given instigator is applied
	 *
	 * @param StatusEffect The class of the status effect to check for
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 * @return True if the status effect is applied
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasStatusEffectFromInstigator(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Instigator) const;

	/**
	 * Checks if there are any status effects applied
	 *
	 * @return True if there is at least one status effect applied
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasAnyStatusEffect() const;

	/**
	 * Checks if there are any status effects applied by the specified instigator
	 *
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 * @return True if there is at least one status effect from the given instigator applied
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasAnyStatusEffectFromInstigator(AActor* Instigator) const;

	/**
	 * Determines if any status effect of the specific type is applied to the actor
	 *
	 * @param StatusEffectType The type of the status effect to check for
	 * @return True if at least one status effect of the given type is applied
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasAnyStatusEffectOfType(const EStatusEffectType StatusEffectType) const;

	/**
	 * Checks if there are any status effects of a specific type applied by a specified instigator
	 *
	 * @param StatusEffectType The type of the status effect to check for
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 * @return True if there is at least one status effect of the specific type applied by the given instigator
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasAnyStatusEffectOfTypeFromInstigator(const EStatusEffectType StatusEffectType, AActor* Instigator) const;

	/**
	 * Retrieves a status effect of the specified class
	 *
	 * @param StatusEffect The class of the status effect to retrieve.
	 * @return A pointer to the status effect object instance. Can be nullptr
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects")
	UStatusEffectBase* GetStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect) const;

	/**
	 * Retrieves a status effect of the specified class that was applied by the given instigator
	 *
	 * @param StatusEffect The class of the status effect to retrieve
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 * @return A pointer to the status effect object instance. Can be nullptr
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects")
	UStatusEffectBase* GetStatusEffectFromInstigator(TSubclassOf<UStatusEffectBase> StatusEffect,
	                                                 AActor* Instigator) const;

	/**
	 * Retrieves all currently applied status effects
	 *
	 * @param OutStatusEffects The array to be populated with matching status effects
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffects(TArray<UStatusEffectBase*>& OutStatusEffects) const;

	/**
	 * Retrieves all status effects of a specific class
	 *
	 * @param OutStatusEffects The array to be populated with matching status effects
	 * @param StatusEffect The class of status effects to filter and retrieve
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffectsOfClass(TArray<UStatusEffectBase*>& OutStatusEffects,
	                                TSubclassOf<UStatusEffectBase> StatusEffect) const;

	/**
	 * Retrieves all status effects applied by a specified instigator
	 *
	 * @param OutStatusEffects The array to be populated with matching status effects
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffectsFromInstigator(TArray<UStatusEffectBase*>& OutStatusEffects, AActor* Instigator) const;

	/**
	 * Retrieves all status effects of a specific class applied by a given instigator
	 *
	 * @param OutStatusEffects The array to be populated with matching status effects
	 * @param StatusEffect The class of status effects to filter and retrieve
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffectsOfClassFromInstigator(TArray<UStatusEffectBase*>& OutStatusEffects,
	                                              TSubclassOf<UStatusEffectBase> StatusEffect,
	                                              AActor* Instigator) const;

	/**
	 * Retrieves all status effects of a specified type
	 *
	 * @param OutStatusEffects The array to be populated with matching status effects
	 * @param StatusEffectType The type of status effects to filter and retrieve
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffectsOfType(TArray<UStatusEffectBase*>& OutStatusEffects,
	                               EStatusEffectType StatusEffectType) const;

	/**
	 * Retrieves all status effects of a specific type applied by the specified instigator
	 *
	 * @param OutStatusEffects An array to be populated with the filtered status effects
	 * @param StatusEffectType The type of status effects to filter and retrieve
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffectsOfTypeFromInstigator(TArray<UStatusEffectBase*>& OutStatusEffects,
	                                             EStatusEffectType StatusEffectType,
	                                             AActor* Instigator) const;

private:
	UPROPERTY(VisibleInstanceOnly, Category="StatusEffects")
	TArray<UStatusEffectBase*> AppliedStatusEffects;

	UFUNCTION()
	void HandleStatusEffectDeactivated(UStatusEffectBase* StatusEffect, AActor* Deactivator);

	UFUNCTION()
	void HandleStatusEffectRefreshed(UStatusEffectBase* StatusEffect);

	UStatusEffectBase* CreateNewStatusEffect(const TSubclassOf<UStatusEffectBase>& StatusEffect, AActor* Instigator);

	static void RefreshGivenStatusEffects(TArray<UStatusEffectBase*>& StatusEffects);

	static void RemoveGivenStatusEffects(TArray<UStatusEffectBase*>& StatusEffects, AActor* Remover);
};
