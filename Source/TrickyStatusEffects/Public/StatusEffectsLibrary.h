// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"
#include "StatusEffectsLibrary.generated.h"

class UStatusEffectBase;
enum class EStatusEffectType : uint8;
class UStatusEffectsManagerComponent;
/**
 * 
 */
UCLASS()
class TRICKYSTATUSEFFECTS_API UStatusEffectsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Retrieves the Status Effects Manager Component attached to the given actor
	 *
	 * @param Target The actor from which to retrieve the Status Effects Manager Component
	 * @return A pointer to the Status Effects Manager Component. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static UStatusEffectsManagerComponent* GetStatusEffectsManager(AActor* Target);

	/**
	 * Applies a status effect to a give actor
	 * If the effect is already applied and its scope is not PerInstance, it will refresh it
	 *
	 * @param Target The actor to which the status effect will be applied
	 * @param StatusEffect The class of the status effect to be applied
	 * @param Instigator The actor which applied a status effect. Can be nullptr
	 * @return A pointer to the applied status effect object. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static UStatusEffectBase* ApplyStatusEffect(AActor* Target,
	                                            TSubclassOf<UStatusEffectBase> StatusEffect,
	                                            AActor* Instigator);

	/**
	 * Refreshes all applied status effects
	 * 
	 * @param Target The actor which status effects will be refreshed
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RefreshAllStatusEffects(AActor* Target);

	/**
	 * Refreshes all applied status effects of the specified class
	 *
	 * @param Target The actor which status effects will be refreshed
	 * @param StatusEffect The class of the status effects to be refreshed
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RefreshAllStatusEffectsOfClass(AActor* Target, TSubclassOf<UStatusEffectBase> StatusEffect);

	/**
	 * Refreshes all status effects applied by a specified instigator
	 *
	 * @param Target The actor which status effects will be refreshed
	 * @param Instigator The actor which applied status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RefreshAllStatusEffectsFromInstigator(AActor* Target, AActor* Instigator);

	/**
	 * Refreshes all active status effects of a specified class that were applied by a specified instigator
	 *
	 * @param Target The actor which status effects will be refreshed
	 * @param StatusEffect The class of the status effects to be refreshed
	 * @param Instigator The actor which applied status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RefreshAllStatusEffectsOfClassFromInstigator(AActor* Target,
	                                                         TSubclassOf<UStatusEffectBase> StatusEffect,
	                                                         AActor* Instigator);

	/**
	 * Refreshes all applied status effects of the specific type
	 * 
	 * @param Target The actor which status effects will be refreshed
	 * @param StatusEffectType The type of status effects to refresh
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RefreshAllStatusEffectsOfType(AActor* Target, const EStatusEffectType StatusEffectType);

	/**
	 * Refreshes all active status effects of the specific type that were applied by a specified instigator
	 * 
	 * @param Target The actor which status effects will be refreshed
	 * @param StatusEffectType The type of status effects to refresh
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RefreshAllStatusEffectsOfTypeFromInstigator(AActor* Target,
	                                                        const EStatusEffectType StatusEffectType,
	                                                        AActor* Instigator);

	/**
	 * Removes a specified status effect from applied status effects.
	 * 
	 * @param Target The actor from which the status effect will be removed
	 * @param StatusEffect The class of the status effect to remove
	 * @param Remover The actor attempting to remove the status effect. Can be nullptr
	 * @return True if the status effect was successfully removed
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool RemoveStatusEffect(AActor* Target, TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Remover);

	/**
	 * Removes a specified status effect applied by a specified instigator.
	 *
	 * @param Target The actor from which the status effect will be removed
	 * @param StatusEffect The class of the status effect to remove
	 * @param Instigator The actor which applied the status effect. Can be nullptr
	 * @param Remover The actor attempting to remove the status effect. Can be nullptr
	 * @return True if the status effect was successfully removed
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool RemoveStatusEffectFromInstigator(AActor* Target,
	                                             TSubclassOf<UStatusEffectBase> StatusEffect,
	                                             AActor* Instigator,
	                                             AActor* Remover);

	/**
	 * Removes all currently applied status effects
	 *
	 * @param Target The actor from which status effects will be removed
	 * @param Remover The actor responsible for removing the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RemoveAllStatusEffects(AActor* Target, AActor* Remover);

	/**
	 * Removes all status effects applied by a specified instigator
	 *
	 * @param Target The actor from which status effects will be removed
	 * @param Instigator The actor that originally applied the status effects. Can be nullptr
	 * @param Remover The actor responsible for removing the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RemoveAllStatusEffectsFromInstigator(AActor* Target, AActor* Instigator, AActor* Remover);

	/**
	 * Removes all applied status effects of the specified class
	 *
	 * @param Target The actor from which status effects will be removed
	 * @param StatusEffect The class of the status effects to remove
	 * @param Remover The actor responsible for removing the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RemoveAllStatusEffectsOfClass(AActor* Target,
	                                          const TSubclassOf<UStatusEffectBase> StatusEffect,
	                                          AActor* Remover);

	/**
	 * Removes all applied status effects of the specified class applied by a specified instigator
	 *
	 * @param Target The actor from which status effects will be removed
	 * @param StatusEffect The class of the status effects to remove
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 * @param Remover The actor responsible for removing the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RemoveAllStatusEffectsOfClassFromInstigator(AActor* Target,
	                                                        TSubclassOf<UStatusEffectBase> StatusEffect,
	                                                        AActor* Instigator,
	                                                        AActor* Remover);

	/**
	 * Removes all applied status effects of a specific type
	 *
	 * @param Target The actor from which status effects will be removed
	 * @param StatusEffectType The type of status effects to remove
	 * @param Remover The actor responsible for removing the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RemoveAllStatusEffectsOfType(AActor* Target, EStatusEffectType StatusEffectType, AActor* Remover);

	/**
	 * Removes all applied status effects of a specific type applied by a specified instigator
	 * 
	 * @param Target The actor from which status effects will be removed
	 * @param StatusEffectType The type of status effect to remove
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 * @param Remover The actor responsible for removing the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RemoveAllStatusEffectsOfTypeFromInstigator(AActor* Target,
	                                                       EStatusEffectType StatusEffectType,
	                                                       AActor* Instigator,
	                                                       AActor* Remover);

	/**
	 * Checks if a specified status effect is currently applied to the actor
	 *
	 * @param Target The actor to check for status effects
	 * @param StatusEffect The class of the status effect to check for
	 * @return True if the status effect is applied
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool HasStatusEffect(AActor* Target, TSubclassOf<UStatusEffectBase> StatusEffect);

	/**
	 * Checks if a specified status effect from a given instigator is applied
	 *
	 * @param Target The actor to check for status effects
	 * @param StatusEffect The class of the status effect to check for
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 * @return True if the status effect is applied
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool HasStatusEffectFromInstigator(AActor* Target,
	                                          TSubclassOf<UStatusEffectBase> StatusEffect,
	                                          AActor* Instigator);

	/**
	 * Checks if there are any status effects applied
	 *
	 * @param Target The actor to check for status effects
	 * @return True if there is at least one status effect applied
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool HasAnyStatusEffect(AActor* Target);

	/**
	 * Checks if there are any status effects applied by the specified instigator
	 * 
	 * @param Target The actor to check for status effects*
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 * @return True if there is at least one status effect from the given instigator applied
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool HasAnyStatusEffectFromInstigator(AActor* Target, AActor* Instigator);

	/**
	 * Determines if any status effect of the specific type is applied to the actor
	 *
	 * @param Target The actor to check for status effects
	 * @param StatusEffectType The type of the status effect to check for
	 * @return True if at least one status effect of the given type is applied
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool HasAnyStatusEffectOfType(AActor* Target, const EStatusEffectType StatusEffectType);

	/**
	 * Checks if there are any status effects of a specific type applied by a specified instigator
	 * 
	 * @param Target The actor to check for status effects
	 * @param StatusEffectType The type of the status effect to check for
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 * @return True if there is at least one status effect of the specific type applied by the given instigator
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool HasAnyStatusEffectOfTypeFromInstigator(AActor* Target,
	                                                   const EStatusEffectType StatusEffectType,
	                                                   AActor* Instigator);

	/**
	 * Retrieves a status effect of the specified class
	 *
	 * @param Target The actor to retrieve status effects from
	 * @param StatusEffect The class of the status effect to retrieve.
	 * @return A pointer to the status effect object instance. Can be nullptr
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static UStatusEffectBase* GetStatusEffect(AActor* Target, TSubclassOf<UStatusEffectBase> StatusEffect);

	/**
	 * Retrieves a status effect of the specified class that was applied by the given instigator
	 *
	 * @param Target The actor to retrieve status effects from
	 * @param StatusEffect The class of the status effect to retrieve
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 * @return A pointer to the status effect object instance. Can be nullptr
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static UStatusEffectBase* GetStatusEffectFromInstigator(AActor* Target,
	                                                        TSubclassOf<UStatusEffectBase> StatusEffect,
	                                                        AActor* Instigator);

	/**
	 * Retrieves all currently applied status effects
	 *
	 * @param Target The actor to retrieve status effects from
	 * @param OutStatusEffects The array to be populated with matching status effects
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static void GetAllStatusEffects(AActor* Target, TArray<UStatusEffectBase*>& OutStatusEffects);

	/**
	 * Retrieves all status effects of a specific class
	 *
	 * @param Target The actor to retrieve status effects from
	 * @param OutStatusEffects The array to be populated with matching status effects
	 * @param StatusEffect The class of status effects to filter and retrieve
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static void GetAllStatusEffectsOfClass(AActor* Target,
	                                       TArray<UStatusEffectBase*>& OutStatusEffects,
	                                       TSubclassOf<UStatusEffectBase> StatusEffect);

	/**
	 * Retrieves all status effects applied by a specified instigator
	 *
	 * @param Target The actor to retrieve status effects from
	 * @param OutStatusEffects The array to be populated with matching status effects
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static void GetAllStatusEffectsFromInstigator(AActor* Target,
	                                              TArray<UStatusEffectBase*>& OutStatusEffects,
	                                              AActor* Instigator);

	/**
	 * Retrieves all status effects of a specific class applied by a given instigator
	 *
	 * @param Target The actor to retrieve status effects from
	 * @param OutStatusEffects The array to be populated with matching status effects
	 * @param StatusEffect The class of status effects to filter and retrieve
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static void GetAllStatusEffectsOfClassFromInstigator(AActor* Target,
	                                                     TArray<UStatusEffectBase*>& OutStatusEffects,
	                                                     TSubclassOf<UStatusEffectBase> StatusEffect,
	                                                     AActor* Instigator);


	/**
	 * Retrieves all status effects of a specified type
	 *
	 * @param Target The actor to retrieve status effects from
	 * @param OutStatusEffects The array to be populated with matching status effects
	 * @param StatusEffectType The type of status effects to filter and retrieve
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static void GetAllStatusEffectsOfType(AActor* Target,
	                                      TArray<UStatusEffectBase*>& OutStatusEffects,
	                                      EStatusEffectType StatusEffectType);

	/**
	 * Retrieves all status effects of a specific type applied by the specified instigator
	 *
	 * @param Target The actor to retrieve status effects from
	 * @param OutStatusEffects An array to be populated with the filtered status effects
	 * @param StatusEffectType The type of status effects to filter and retrieve
	 * @param Instigator The actor which applied the status effects. Can be nullptr
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static void GetAllStatusEffectsOfTypeFromInstigator(AActor* Target,
	                                                    TArray<UStatusEffectBase*>& OutStatusEffects,
	                                                    EStatusEffectType StatusEffectType,
	                                                    AActor* Instigator);
};
