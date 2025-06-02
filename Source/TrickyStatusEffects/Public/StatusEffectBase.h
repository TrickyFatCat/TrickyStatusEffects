// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StatusEffectBase.generated.h"

class UStatusEffectsManagerComponent;

UENUM(BlueprintType)
enum class EStatusEffectType : uint8
{
	Neutral,
	Positive,
	Negative
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusEffectDeactivated,
                                             UStatusEffectBase*, StatusEffect,
                                             AActor*, Deactivator);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TRICKYSTATUSEFFECTS_API UStatusEffectBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnStatusEffectDeactivated OnStatusEffectDeactivated;
	
	virtual bool IsTickable() const override;

	virtual bool IsTickableWhenPaused() const override;

	virtual bool IsTickableInEditor() const override;

	virtual ETickableTickType GetTickableTickType() const override;

	virtual UWorld* GetTickableGameObjectWorld() const override;

	bool ActivateStatusEffect(AActor* Instigator, AActor* Target);

	bool DeactivateStatusEffect(AActor* Deactivator);

	UFUNCTION(BlueprintGetter, Category = "StatusEffects")
	bool GetIsInfinite() const { return bIsInfinite; }

	UFUNCTION(BlueprintGetter, Category = "StatusEffects")
	float GetDuration() const { return Duration; }

	UFUNCTION(BlueprintPure, Category = "StatusEffects")
	float GetRemainingTime() const;

	UFUNCTION(BlueprintPure, Category = "StatusEffects")
	float GetElapsedTime() const;

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
		return true;
	}

private:
	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

	UPROPERTY(EditDefaultsOnly, Category="Tick")
	bool bTickEffect = false;

	UPROPERTY(EditDefaultsOnly, Category="Tick", meta=(ClampMin=0.0f, UIMin=0.01f, EditCondition="bTickEffect"))
	float TickInterval = 0.0f;

	float TickDuration = -1.0f;

	/**
	 * The last frame number we were ticked.
	 * We don't want to tick multiple times per frame
	 */
	uint32 LastFrameNumberWeTicked = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, Category="StatusEffect")
	EStatusEffectType EffectType = EStatusEffectType::Neutral;

	UPROPERTY(EditDefaultsOnly, Category="StatusEffect")
	bool bIsInfinite = true;

	UPROPERTY(EditDefaultsOnly, Category="StatusEffect", meta=(ClampMin=0.0f, UIMin=0.0f, EditCondition="!bIsInfinite"))
	float Duration = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="StatusEffect")
	float StatusEffectTimer = -1.0f;

	UPROPERTY(BlueprintGetter=GetTargetActor, Category="StatusEffect")
	AActor* TargetActor = nullptr;

	UPROPERTY(BlueprintGetter=GetInstigatorActor, Category="StatusEffect")
	AActor* InstigatorActor = nullptr;

	UPROPERTY(BlueprintGetter=GetOwningManager, Category="StatusEffect")
	TObjectPtr<UStatusEffectsManagerComponent> OwningManager = nullptr;

	void ProcessTick(float DeltaTime);

	void ProcessEffectDurationTimer(float DeltaTime);
};
