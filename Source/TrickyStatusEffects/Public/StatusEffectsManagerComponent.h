// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "StatusEffectBase.h"
#include "Components/ActorComponent.h"
#include "StatusEffectsManagerComponent.generated.h"


class UStatusEffectBase;

UCLASS(ClassGroup=(TrickyStatusEffects), meta=(BlueprintSpawnableComponent))
class TRICKYSTATUSEFFECTS_API UStatusEffectsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatusEffectsManagerComponent();

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	UStatusEffectBase* ApplyStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffects();

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsByClass(TSubclassOf<UStatusEffectBase> StatusEffect) const;

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsByInstigator(AActor* Instigator) const;

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsByClassOfInstigator(TSubclassOf<UStatusEffectBase> StatusEffect,
	                                                AActor* Instigator) const;

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsByType(EStatusEffectType StatusEffectType);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsByTypeOfInstigator(EStatusEffectType StatusEffectType, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool RemoveStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Remover);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool RemoveStatusEffectByInstigator(TSubclassOf<UStatusEffectBase> StatusEffect,
	                                    AActor* Remover,
	                                    AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffects(AActor* Remover);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsByInstigator(AActor* Remover, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsByClass(AActor* Remover, TSubclassOf<UStatusEffectBase> StatusEffect);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsByClassOfInstigator(AActor* Remover,
	                                               TSubclassOf<UStatusEffectBase> StatusEffect,
	                                               AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsByType(AActor* Remover, EStatusEffectType StatusEffectType);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsByTypeOfInstigator(AActor* Remover,
	                                              EStatusEffectType StatusEffectType,
	                                              AActor* Instigator);

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasStatusEffectByInstigator(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Instigator) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasAnyStatusEffects() const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasAnyStatusEffectOfInstigator(AActor* Instigator) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	UStatusEffectBase* GetStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	UStatusEffectBase* GetStatusEffectByInstigator(TSubclassOf<UStatusEffectBase> StatusEffect,
	                                               AActor* Instigator) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffects(TArray<UStatusEffectBase*>& OutStatusEffects) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffectsByClass(TArray<UStatusEffectBase*>& OutStatusEffects,
	                                TSubclassOf<UStatusEffectBase> StatusEffects) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffectsByInstigator(TArray<UStatusEffectBase*>& OutStatusEffects, AActor* Instigator) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffectsByClassOfInstigator(TArray<UStatusEffectBase*>& OutStatusEffects,
	                                            TSubclassOf<UStatusEffectBase> StatusEffects,
	                                            AActor* Instigator) const;


	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffectsByType(TArray<UStatusEffectBase*>& OutStatusEffects,
	                               EStatusEffectType StatusEffectType) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffectsByTypeOfInstigator(TArray<UStatusEffectBase*>& OutStatusEffects,
	                                           EStatusEffectType StatusEffectType,
	                                           AActor* Instigator) const;

private:
	UPROPERTY(VisibleInstanceOnly, Category="StatusEffects")
	TArray<UStatusEffectBase*> AppliedStatusEffects;

	UFUNCTION()
	void HandleStatusEffectDeactivated(UStatusEffectBase* StatusEffect, AActor* Deactivator);

	UStatusEffectBase* CreateNewStatusEffect(const TSubclassOf<UStatusEffectBase>& StatusEffect, AActor* Instigator);

	static void RefreshGivenStatusEffects(TArray<UStatusEffectBase*>& StatusEffects);
};
