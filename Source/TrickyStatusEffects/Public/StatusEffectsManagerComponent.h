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

UCLASS(ClassGroup=(TrickyStatusEffects), meta=(BlueprintSpawnableComponent))
class TRICKYSTATUSEFFECTS_API UStatusEffectsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatusEffectsManagerComponent();

	UPROPERTY(BlueprintAssignable, Category="StatusEffects")
	FOnStatusEffectAppliedDynamicSignature OnStatusEffectApplied;

	UPROPERTY(BlueprintAssignable, Category="StatusEffects")
	FOnStatusEffectRemovedDynamicSignature OnStatusEffectRemoved;

	UPROPERTY(BlueprintAssignable, Category="StatusEffects")
	FOnStatusEffectRefreshedDynamicSignature OnStatusEffectRefreshed;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	UStatusEffectBase* ApplyStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffects();

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsOfClass(TSubclassOf<UStatusEffectBase> StatusEffect) const;

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsFromInstigator(AActor* Instigator) const;

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsOfClassFromInstigator(TSubclassOf<UStatusEffectBase> StatusEffect,
	                                                AActor* Instigator) const;

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsOfType(const EStatusEffectType StatusEffectType) const;

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RefreshAllStatusEffectsOfTypeFromInstigator(const EStatusEffectType StatusEffectType, AActor* Instigator) const;

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool RemoveStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Remover) const;

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool RemoveStatusEffectFromInstigator(TSubclassOf<UStatusEffectBase> StatusEffect,
	                                    AActor* Remover,
	                                    AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffects(AActor* Remover);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsFromInstigator(AActor* Remover, AActor* Instigator) const;

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsOfClass(AActor* Remover, TSubclassOf<UStatusEffectBase> StatusEffect) const;

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsOfClassFromInstigator(AActor* Remover,
	                                               TSubclassOf<UStatusEffectBase> StatusEffect,
	                                               AActor* Instigator) const;

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsOfType(AActor* Remover, EStatusEffectType StatusEffectType);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	void RemoveAllStatusEffectsOfTypeFromInstigator(AActor* Remover,
	                                              EStatusEffectType StatusEffectType,
	                                              AActor* Instigator);

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasStatusEffectFromInstigator(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Instigator) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasAnyStatusEffect() const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasAnyStatusEffectFromInstigator(AActor* Instigator) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasAnyStatusEffectOfType(const EStatusEffectType StatusEffectType) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasAnyStatusEffectOfTypeFromInstigator(const EStatusEffectType StatusEffectType, AActor* Instigator) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	UStatusEffectBase* GetStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	UStatusEffectBase* GetStatusEffectFromInstigator(TSubclassOf<UStatusEffectBase> StatusEffect,
	                                               AActor* Instigator) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffects(TArray<UStatusEffectBase*>& OutStatusEffects) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffectsOfClass(TArray<UStatusEffectBase*>& OutStatusEffects,
	                                TSubclassOf<UStatusEffectBase> StatusEffect) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffectsFromInstigator(TArray<UStatusEffectBase*>& OutStatusEffects, AActor* Instigator) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffectsOfClassFromInstigator(TArray<UStatusEffectBase*>& OutStatusEffects,
	                                            TSubclassOf<UStatusEffectBase> StatusEffect,
	                                            AActor* Instigator) const;


	UFUNCTION(BlueprintPure, Category="StatusEffects")
	void GetAllStatusEffectsOfType(TArray<UStatusEffectBase*>& OutStatusEffects,
	                               EStatusEffectType StatusEffectType) const;

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
