// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/Engine.h"
#include "Tickable.h"
#include "StatusEffectBase.generated.h"

class UStatusEffectsManagerComponent;

UENUM(BlueprintType)
enum class EStatusEffectType : uint8
{
	Neutral,
	Buff,
	Debuff
};

UENUM(BlueprintType)
enum class EStatusEffectTimerRefreshBehavior : uint8
{
	Ignore,
	Restart,
	Extend
};

UENUM(BlueprintType)
enum class EStatusEffectStacksRefreshBehavior : uint8
{
	Ignore,
	Reset,
	Increase
};

UENUM(BlueprintType)
enum class EStatusEffectScope : uint8
{
	PerInstance,
	PerTarget,
	PerInstigator
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeactivatedDynamicSignature,
                                             UStatusEffectBase*, StatusEffect,
                                             AActor*, Deactivator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRefreshedDynamicSignature,
                                            UStatusEffectBase*, StatusEffect);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStacksIncreasedDynamicSignature,
                                             UStatusEffectBase*, StatusEffect,
                                             int32, NewStacks);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStacksDecreasedDynamicSignature,
                                             UStatusEffectBase*, StatusEffect,
                                             int32, NewStacks);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract, HideDropdown)
class TRICKYSTATUSEFFECTS_API UStatusEffectBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnDeactivatedDynamicSignature OnStatusEffectDeactivated;

	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnRefreshedDynamicSignature OnStatusEffectRefreshed;

	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnStacksIncreasedDynamicSignature OnStatusEffectStacksIncreased;

	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnStacksDecreasedDynamicSignature OnStatusEffectStacksDecreased;

	virtual bool IsTickable() const override;

	virtual bool IsTickableWhenPaused() const override;

	virtual bool IsTickableInEditor() const override;

	virtual ETickableTickType GetTickableTickType() const override;

	virtual UWorld* GetTickableGameObjectWorld() const override;

	bool Activate(UStatusEffectsManagerComponent* TargetManagerComponent, AActor* Instigator);

	void Refresh();

	void Deactivate(AActor* Deactivator);

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	EStatusEffectType GetEffectType() const { return EffectType; };

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	EStatusEffectScope GetEffectScope() const { return EffectScope; }

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	bool GetIsInfinite() const { return bIsInfinite; }

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	float GetDuration() const { return Duration; }

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	EStatusEffectTimerRefreshBehavior GetTimerBehavior() const { return TimerBehavior; }

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	float GetMaxDuration() const { return MaxDuration; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	float GetRemainingTime() const;

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	float GetElapsedTime() const;

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	bool GetIsStackable() const { return bIsStackable; }

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	int32 GetInitialStacks() const { return InitialStacks; }

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	int32 GetMaxStacks() const { return MaxStacks; }

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	int32 GetCurrentStacks() const { return CurrentStacks; }

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	EStatusEffectStacksRefreshBehavior GetStacksBehavior() const { return StacksBehavior; }

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	int32 GetDeltaStacks() const { return DeltaStacks; }

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	AActor* GetTargetActor() const { return TargetActor; }

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	AActor* GetInstigatorActor() const { return InstigatorActor; }

	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	UStatusEffectsManagerComponent* GetOwningManager() const { return OwningManager; }
	
	UFUNCTION(BlueprintCallable, Category="StatusEffect")
	bool IncreaseStacks(const int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category="StatusEffect")
	bool DecreaseStacks(const int32 Amount = 1);
	
protected:
	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	bool CanBeActivated();

	virtual bool CanBeActivated_Implementation()
	{
		return true;
	}

	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	void ActivateEffect();

	virtual void ActivateEffect_Implementation()
	{
	}

	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	void TickEffect(float DeltaTime);

	virtual void TickEffect_Implementation(float DeltaTime)
	{
	}

	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	void DeactivateEffect(AActor* Deactivator);

	virtual void DeactivateEffect_Implementation(AActor* Deactivator)
	{
	}

	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	void RefreshEffect();

	virtual void RefreshEffect_Implementation()
	{
	}

	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	void HandleStacksIncreased(const int32 Amount = 1);

	virtual void HandleStacksIncreased_Implementation(const int32 Amount = 1)
	{
	}

	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	void HandleStacksDecreased(const int32 Amount = 1);

	virtual void HandleStacksDecreased_Implementation(const int32 Amount = 1)
	{
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

	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetEffectType, Category="General")
	EStatusEffectType EffectType = EStatusEffectType::Neutral;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetEffectScope, Category="General")
	EStatusEffectScope EffectScope = EStatusEffectScope::PerTarget;

	UPROPERTY(EditDefaultsOnly, Category="Duration")
	bool bIsInfinite = true;

	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetDuration,
		Category="Duration",
		meta=(ClampMin=0.0f, UIMin=0.0f, EditCondition="!bIsInfinite"))
	float Duration = 5.0f;

	UPROPERTY()
	float StatusEffectTimer = -1.0f;

	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetTimerBehavior,
		Category="Duration",
		meta=(EditCondition="!bIsInfinite"))
	EStatusEffectTimerRefreshBehavior TimerBehavior = EStatusEffectTimerRefreshBehavior::Ignore;

	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetMaxDuration,
		Category="Duration",
		meta=(ClampMin=0.0f, UIMin=0.0f,
			EditCondition="!bIsInfinite && TimerBehavior == EStatusEffectTimerRefreshBehavior::Extend"))
	float MaxDuration = 10.0f;

	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetMaxDuration,
		Category="Duration",
		meta=(ClampMin=0.0f, UIMin=0.0f,
			EditCondition="!bIsInfinite && TimerBehavior == EStatusEffectTimerRefreshBehavior::Extend"))
	float DeltaDuration = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetIsStackable, Category="Stacks")
	bool bIsStackable = false;

	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetInitialStacks,
		Category="Stacks",
		meta=(ClampMin=1, UIMin=1, EditCondition="bIsStackable"))
	int32 InitialStacks = 1;

	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetMaxStacks,
		Category="Stacks",
		meta=(ClampMin=1, UIMin=1, EditCondition="bIsStackable"))
	int32 MaxStacks = 5;

	UPROPERTY(BlueprintGetter=GetCurrentStacks, Category="Stacks")
	int32 CurrentStacks = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetStacksBehavior, Category="Stacks")
	EStatusEffectStacksRefreshBehavior StacksBehavior = EStatusEffectStacksRefreshBehavior::Ignore;

	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetDeltaStacks,
		Category="Stacks",
		meta=(ClampMin=1, UIMin=1,
			EditCondition="bIsStackable && StacksBehavior == EStatusEffectStacksRefreshBehavior::Increase"))
	int32 DeltaStacks = 1;

	UPROPERTY(BlueprintGetter=GetTargetActor, Category="StatusEffect")
	AActor* TargetActor = nullptr;

	UPROPERTY(BlueprintGetter=GetInstigatorActor, Category="StatusEffect")
	AActor* InstigatorActor = nullptr;

	UPROPERTY(BlueprintGetter=GetOwningManager, Category="StatusEffect")
	TObjectPtr<UStatusEffectsManagerComponent> OwningManager = nullptr;

	void ProcessTick(float DeltaTime);

	void ProcessEffectDurationTimer(float DeltaTime);

	void RefreshTimer();

	void RefreshStacks();
};
