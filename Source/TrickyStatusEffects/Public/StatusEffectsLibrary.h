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
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static UStatusEffectsManagerComponent* GetStatusEffectsManager(AActor* Target);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static UStatusEffectBase* ApplyStatusEffect(AActor* Target,
	                                            TSubclassOf<UStatusEffectBase> StatusEffect,
	                                            AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RefreshAllStatusEffects(AActor* Target);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RefreshAllStatusEffectsOfClass(AActor* Target, TSubclassOf<UStatusEffectBase> StatusEffect);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RefreshAllStatusEffectsFromInstigator(AActor* Target, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RefreshAllStatusEffectsOfClassFromInstigator(AActor* Target,
	                                                         TSubclassOf<UStatusEffectBase> StatusEffect,
	                                                         AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RefreshAllStatusEffectsOfType(AActor* Target, const EStatusEffectType StatusEffectType);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RefreshAllStatusEffectsOfTypeFromInstigator(AActor* Target,
	                                                        const EStatusEffectType StatusEffectType,
	                                                        AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool RemoveStatusEffect(AActor* Target, TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Remover);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool RemoveStatusEffectFromInstigator(AActor* Target,
	                                             TSubclassOf<UStatusEffectBase> StatusEffect,
	                                             AActor* Instigator,
	                                             AActor* Remover);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RemoveAllStatusEffects(AActor* Target, AActor* Remover);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RemoveAllStatusEffectsFromInstigator(AActor* Target, AActor* Instigator, AActor* Remover);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RemoveAllStatusEffectsOfClass(AActor* Target,
	                                          const TSubclassOf<UStatusEffectBase> StatusEffect,
	                                          AActor* Remover);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RemoveAllStatusEffectsOfClassFromInstigator(AActor* Target,
	                                                        TSubclassOf<UStatusEffectBase> StatusEffect,
	                                                        AActor* Instigator,
	                                                        AActor* Remover);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RemoveAllStatusEffectsOfType(AActor* Target, EStatusEffectType StatusEffectType, AActor* Remover);

	UFUNCTION(BlueprintCallable, Category="StatusEffects", meta=(WorldContext="Target"))
	static void RemoveAllStatusEffectsOfTypeFromInstigator(AActor* Target,
	                                                       EStatusEffectType StatusEffectType,
	                                                       AActor* Instigator,
	                                                       AActor* Remover);

	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool HasStatusEffect(AActor* Target, TSubclassOf<UStatusEffectBase> StatusEffect);

	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool HasStatusEffectFromInstigator(AActor* Target,
	                                          TSubclassOf<UStatusEffectBase> StatusEffect,
	                                          AActor* Instigator);

	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool HasAnyStatusEffect(AActor* Target);

	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool HasAnyStatusEffectFromInstigator(AActor* Target, AActor* Instigator);

	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool HasAnyStatusEffectOfType(AActor* Target, const EStatusEffectType StatusEffectType);

	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static bool HasAnyStatusEffectOfTypeFromInstigator(AActor* Target,
	                                                   const EStatusEffectType StatusEffectType,
	                                                   AActor* Instigator);

	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static UStatusEffectBase* GetStatusEffect(AActor* Target, TSubclassOf<UStatusEffectBase> StatusEffect);

	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static UStatusEffectBase* GetStatusEffectFromInstigator(AActor* Target,
	                                                        TSubclassOf<UStatusEffectBase> StatusEffect,
	                                                        AActor* Instigator);

	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static void GetAllStatusEffects(AActor* Target, TArray<UStatusEffectBase*>& OutStatusEffects);

	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static void GetAllStatusEffectsOfClass(AActor* Target,
	                                       TArray<UStatusEffectBase*>& OutStatusEffects,
	                                       TSubclassOf<UStatusEffectBase> StatusEffect);

	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static void GetAllStatusEffectsFromInstigator(AActor* Target,
	                                              TArray<UStatusEffectBase*>& OutStatusEffects,
	                                              AActor* Instigator);

	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static void GetAllStatusEffectsOfClassFromInstigator(AActor* Target,
	                                                     TArray<UStatusEffectBase*>& OutStatusEffects,
	                                                     TSubclassOf<UStatusEffectBase> StatusEffect,
	                                                     AActor* Instigator);


	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static void GetAllStatusEffectsOfType(AActor* Target,
	                                      TArray<UStatusEffectBase*>& OutStatusEffects,
	                                      EStatusEffectType StatusEffectType);

	UFUNCTION(BlueprintPure, Category="StatusEffects", meta=(WorldContext="Target"))
	static void GetAllStatusEffectsOfTypeFromInstigator(AActor* Target,
	                                                    TArray<UStatusEffectBase*>& OutStatusEffects,
	                                                    EStatusEffectType StatusEffectType,
	                                                    AActor* Instigator);
};
