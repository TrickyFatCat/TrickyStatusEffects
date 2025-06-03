// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
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
	bool RemoveStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Remover);

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect) const;

	UFUNCTION(BlueprintPure, Category="StatusEffects")
	bool HasStatusEffectByInstigator(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Instigator) const;

private:
	UPROPERTY(VisibleInstanceOnly, Category="StatusEffects")
	TArray<UStatusEffectBase*> StatusEffects;

	UStatusEffectBase* GetStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect) const;
	
	UFUNCTION()
	void HandleStatusEffectDeactivated(UStatusEffectBase* StatusEffect, AActor* Deactivator);

	UStatusEffectBase* CreateNewStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Instigator);
};
