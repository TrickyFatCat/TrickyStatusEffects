// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StatusEffectBase.generated.h"

class UStatusEffectsManagerComponent;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TRICKYSTATUSEFFECTS_API UStatusEffectBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual bool IsTickable() const override;

	virtual bool IsTickableWhenPaused() const override;

	virtual bool IsTickableInEditor() const override;

	virtual ETickableTickType GetTickableTickType() const override;

	virtual UWorld* GetTickableGameObjectWorld() const override;

	UFUNCTION()
	bool ActivateStatusEffect(AActor* Instigator, AActor* Target);

	UFUNCTION()
	bool DeactivateStatusEffect(AActor* Deactivator);

	UFUNCTION(BlueprintGetter, Category = "StatusEffects")
	AActor* GetTargetActor() const { return TargetActor; }

	UFUNCTION(BlueprintGetter, Category = "StatusEffects")
	AActor* GetInstigatorActor() const { return InstigatorActor; }

	UFUNCTION(BlueprintGetter, Category = "StatusEffects")
	UStatusEffectsManagerComponent* GetOwningManager() const { return OwningManager; }

protected:
	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	bool ActivateEffect();

	virtual bool ActivateEffect_Implementation()
	{
		return false;
	}

	UFUNCTION(BlueprintNativeEvent, Category = "StatusEffect")
	void TickEffect(float DeltaTime);

	virtual void TickEffect_Implementation(float DeltaTime)
	{
		
	}

	UFUNCTION(BlueprintNativeEvent, Category = "StatusEffect")
	bool DeactivateEffect(AActor* Deactivator);

	virtual bool DeactivateEffect_Implementation(AActor* Deactivator)
	{
		return false;
	}

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
