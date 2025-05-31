// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StatusEffectBase.generated.h"

class UStatusEffectsManagerComponent;
/**
 * 
 */
UCLASS()
class TRICKYSTATUSEFFECTS_API UStatusEffectBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual bool IsTickable() const override;

	virtual bool IsTickableWhenPaused() const override;

	virtual UWorld* GetTickableGameObjectWorld() const override;

	virtual bool ActivateStatusEffect(AActor* Instigator, AActor* Target);

	virtual bool DeactivateStatusEffect(AActor* Deactivator);

	UFUNCTION(BlueprintGetter, Category = "StatusEffects")
	AActor* GetTargetActor() const { return TargetActor; }

	UFUNCTION(BlueprintGetter, Category = "StatusEffects")
	AActor* GetInstigatorActor() const { return InstigatorActor; }

	UFUNCTION(BlueprintGetter, Category = "StatusEffects")
	UStatusEffectsManagerComponent* GetOwningManager() const { return OwningManager; }

protected:
	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	bool ActivationEvent();

	virtual bool ActivationEvent_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "StatusEffect")
	void TickEvent(float DeltaTime);

	virtual void TickEvent_Implementation(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, Category = "StatusEffect")
	bool DeactivationEvent(AActor* Deactivator);

	virtual bool DeactivationEvent_Implementation(AActor* Deactivator);

private:
	virtual void Tick(float DeltaTime) override;
	
	virtual TStatId GetStatId() const override;

	UPROPERTY(BlueprintGetter=GetTargetActor, Category="StatusEffect")
	AActor* TargetActor = nullptr;

	UPROPERTY(BlueprintGetter=GetInstigatorActor, Category="StatusEffect")
	AActor* InstigatorActor = nullptr;

	UPROPERTY(BlueprintGetter=GetOwningManager, Category="StatusEffect")
	TObjectPtr<UStatusEffectsManagerComponent> OwningManager = nullptr;
};
