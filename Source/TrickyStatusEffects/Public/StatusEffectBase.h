// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/Engine.h"
#include "Tickable.h"
#include "StatusEffectBase.generated.h"

class UWorld;
class UStatusEffectsManagerComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogStatusEffect, Log, All)

/**
 * Represents status effect type
 */
UENUM(BlueprintType)
enum class EStatusEffectType : uint8
{
	Neutral,
	Buff,
	Debuff
};

/**
 * Represents duration timer behavior on status effect refresh
 */
UENUM(BlueprintType)
enum class EStatusEffectTimerRefreshBehavior : uint8
{
	Ignore UMETA(ToolTip="No Effect"),
	Reset UMETA(ToolTip="Timer will be reset to initial value"),
	Extend UMETA(ToolTip="Timer will be extended by a delta value")
};

/**
 * Represents stacks behavior on status effect refresh
 */
UENUM(BlueprintType)
enum class EStatusEffectStacksRefreshBehavior : uint8
{
	Ignore UMETA(ToolTip="No Effect"),
	Reset UMETA(ToolTip="Stacks will be reset to initial value"),
	Increase UMETA(ToolTip="Stacks will be increased by a delta value"),
};

/**
 * Represents the scope of a status effect's application and management
 */
UENUM(BlueprintType)
enum class EStatusEffectScope : uint8
{
	PerInstance UMETA(
		ToolTip="Individual instances of status effect will be created each time it is applied to a target."),
	PerTarget UMETA(
		ToolTip="Only one instance of status effect will be created for a target regardless of instigator."),
	PerInstigator UMETA(
		ToolTip="Only one instance of status effect will be created for an instigator regardless of target."),
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
 * Base class for status effects in the game.
 */
UCLASS(Blueprintable, BlueprintType, Abstract, HideDropdown)
class TRICKYSTATUSEFFECTS_API UStatusEffectBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	/**
	 * Called when a status effect was deactivated
	 */
	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnDeactivatedDynamicSignature OnStatusEffectDeactivated;

	/**
	 * Called when a status effect was refreshed
	 */
	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnRefreshedDynamicSignature OnStatusEffectRefreshed;

	/**
	 * Called when status effect's stacks were increased
	 */
	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnStacksIncreasedDynamicSignature OnStatusEffectStacksIncreased;

	/**
	 * Called when status effect's stacks were decreased
	 */
	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnStacksDecreasedDynamicSignature OnStatusEffectStacksDecreased;

	virtual bool IsTickable() const override;

	virtual bool IsTickableWhenPaused() const override;

	virtual bool IsTickableInEditor() const override;

	virtual ETickableTickType GetTickableTickType() const override;

	virtual UWorld* GetTickableGameObjectWorld() const override;
	
#if WITH_ENGINE
	virtual UWorld* GetWorld() const override;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif
#endif

	/**
	 * Activates the status effect on the target manager component.
	 * If activation fails, the object is marked as garbage
	 *
	 * @param TargetManagerComponent The manager component that owns and applies the status effect
	 * @param Instigator The actor responsible for applying the status effect. Can be nullptr
	 * @return true if the status effect was successfully activated
	 */
	bool Activate(UStatusEffectsManagerComponent* TargetManagerComponent, AActor* Instigator);

	/**
	 * Updates the state of the status effect, including its timer, stacks,
	 * and any additional effect logic defined in derived classes.
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffect")
	void Refresh();

	/**
	 * Deactivates the status effect.
	 *
	 * @param Deactivator The actor responsible for deactivating the status effect. Can be nullptr
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffect")
	void Deactivate(AActor* Deactivator);

	/**
	 * Retrieves the type of the status effect.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	EStatusEffectType GetEffectType() const { return EffectType; };

	/**
	 * Retrieves the scope of the status effect.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	EStatusEffectScope GetEffectScope() const { return EffectScope; }

	/**
	 * Checks if the status effect has an infinite duration.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	bool GetIsInfinite() const { return bIsInfinite; }

	/**
	 * Retrieves the duration of the status effect.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	float GetDuration() const { return Duration; }

	/**
	 * Retrieves the current behavior of the status effect's timer on refresh.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	EStatusEffectTimerRefreshBehavior GetTimerBehavior() const { return TimerBehavior; }

	/**
	 * Retrieves the maximum duration of the status effect in seconds.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	float GetMaxDuration() const { return MaxDuration; }

	/**
	 * Retrieves the amount of time by which the status effect's duration can be extended.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	float GetDeltaDuration() const { return DeltaDuration; }

	/**
	 * Retrieves the remaining time for the status effect.
	 * If the status effect has an infinite duration, it returns -1.0f.
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffect")
	float GetRemainingTime() const;

	/**
	 * Retrieves the elapsed time of the status effect.
	 * If the status effect has an infinite duration, it returns -1.0f.
	 */
	UFUNCTION(BlueprintPure, Category="StatusEffect")
	float GetElapsedTime() const;

	/**
	 * Determines whether the status effect can be stacked.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	bool GetIsStackable() const { return bIsStackable; }

	/**
	 * Retrieves the initial number of stacks.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	int32 GetInitialStacks() const { return InitialStacks; }

	/**
	 * Retrieves the maximum number of stacks that the status effect can have.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	int32 GetMaxStacks() const { return MaxStacks; }

	/**
	 * Retrieves the current number of stacks.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	int32 GetCurrentStacks() const { return CurrentStacks; }

	/**
	 * Retrieves the current behavior of the status effect's stacks on refresh.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	EStatusEffectStacksRefreshBehavior GetStacksBehavior() const { return StacksBehavior; }

	/**
	 * Retrieves the delta value for status effect stacks which will be added on refresh.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	int32 GetDeltaStacks() const { return DeltaStacks; }

	/**
	 * Retrieves the target actor affected by the status effect.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	AActor* GetTargetActor() const { return TargetActor; }

	/**
	 * Retrieves the actor that instigated the status effect.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	AActor* GetInstigatorActor() const { return InstigatorActor; }

	/**
	 * Retrieves the owning manager component that manages this status effect.
	 */
	UFUNCTION(BlueprintGetter, Category="StatusEffect")
	UStatusEffectsManagerComponent* GetOwningManager() const { return OwningManager; }

	/**
	 * Increases the number of stacks for the status effect by the given amount.
	 *
	 * @param Amount The number of stacks to add to current stacks. Must be greater than 0
	 * @return true if the stacks were successfully increased
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffect")
	bool IncreaseStacks(const int32 Amount = 1);

	/**
	 * Decreases the number of stacks for the status effect by the given amount.
	 *
	 * @param Amount The number of stacks to subtract from current stacks. Must be greater than 0
	 * @return true if the stacks were successfully decreased
	 */
	UFUNCTION(BlueprintCallable, Category="StatusEffect")
	bool DecreaseStacks(const int32 Amount = 1);

protected:
	/**
	 * Determines whether the status effect can be activated.
	 * Returns true by default.
	 *
	 * @return true if the status effect meets the conditions for activation
	 */
	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	bool CanBeActivated();

	virtual bool CanBeActivated_Implementation()
	{
		return true;
	}

	/**
	 * Runs status effect activation logic
	 */
	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	void ActivateEffect();

	virtual void ActivateEffect_Implementation()
	{
	}

	/**
	 * Runs status effect tick logic
	 *
	 * @param DeltaTime The time elapsed since the last tick
	 */
	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	void TickEffect(float DeltaTime);

	virtual void TickEffect_Implementation(float DeltaTime)
	{
	}

	/**
	 * Runs status effect deactivation logic
	 *
	 * @param Deactivator The actor responsible for deactivating the status effect
	 */
	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	void DeactivateEffect(AActor* Deactivator);

	virtual void DeactivateEffect_Implementation(AActor* Deactivator)
	{
	}

	/**
	 * Runs status effect refresh logic
	 */
	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	void RefreshEffect();

	virtual void RefreshEffect_Implementation()
	{
	}

	/**
	 * Runs status effect logic on stacks increase.
	 *
	 * @param Amount The number of stacks to be added
	 */
	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	void HandleStacksIncreased(const int32 Amount = 1);

	virtual void HandleStacksIncreased_Implementation(const int32 Amount = 1)
	{
	}

	/**
	 * Runs status effect logic on stacks decrease.
	 *
	 * @param Amount The number of stacks to be subtracted
	 */
	UFUNCTION(BlueprintNativeEvent, Category="StatusEffect")
	void HandleStacksDecreased(const int32 Amount = 1);

	virtual void HandleStacksDecreased_Implementation(const int32 Amount = 1)
	{
	}

private:
	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

	/**
	 * Determines if status effect can tick
	 */
	UPROPERTY(EditDefaultsOnly, Category="Tick")
	bool bTickEffect = false;

	/**
	 * Determines the interval between status effect ticks.
	 * If it's less or equal to 0, a tick function will be called each frame. 
	 */
	UPROPERTY(EditDefaultsOnly, Category="Tick", meta=(ClampMin=0.0f, UIMin=0.0f, EditCondition="bTickEffect"))
	float TickInterval = 0.0f;

	/**
	 * Represents the duration of each tick in seconds for a status effect.
	 * A negative value signifies an uninitialized or inactive state.
	 */
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

	/**
	 * Indicates whether the status effect has an infinite duration.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Duration")
	bool bIsInfinite = true;

	/**
	 * Determines status effect duration in seconds
	 */
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetDuration,
		Category="Duration",
		meta=(ClampMin=0.0f, UIMin=0.0f, Delta=1, EditCondition="!bIsInfinite", ForceUnits="Seconds"))
	float Duration = 5.0f;

	UPROPERTY()
	float RemainingDuration = -1.0f;

	/**
	 * Determines status effects calculations on status effect refresh
	 */
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetTimerBehavior,
		Category="Duration",
		meta=(EditCondition="!bIsInfinite"))
	EStatusEffectTimerRefreshBehavior TimerBehavior = EStatusEffectTimerRefreshBehavior::Ignore;

	/**
	 * Determines the maximum duration of the status effect in seconds
	 * Used to limit duration if TimerBehavior set to Extend
	 */
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetMaxDuration,
		Category="Duration",
		meta=(ClampMin=0.0f, UIMin=0.0f, Delta=1, ForceUnits="Seconds",
			EditCondition="!bIsInfinite && TimerBehavior == EStatusEffectTimerRefreshBehavior::Extend"))
	float MaxDuration = 10.0f;

	/**
	 * Determines the amount of time in seconds added to the duration of a status effect when it is extended
	 */
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetDeltaDuration,
		Category="Duration",
		meta=(ClampMin=0.0f, UIMin=0.0f, Delta=1, ForceUnits="Seconds",
			EditCondition="!bIsInfinite && TimerBehavior == EStatusEffectTimerRefreshBehavior::Extend"))
	float DeltaDuration = 5.0f;

	/**
	 * Determines whether the status effect can be stacked
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetIsStackable, Category="Stacks")
	bool bIsStackable = false;

	/**
	 * Defines the initial number of stacks for a status effect on activation
	 */
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetInitialStacks,
		Category="Stacks",
		meta=(ClampMin=1, UIMin=1, EditCondition="bIsStackable"))
	int32 InitialStacks = 1;

	/**
	 * Defines the maximum number of stacks a status effect can have
	 */
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetMaxStacks,
		Category="Stacks",
		meta=(ClampMin=1, UIMin=1, EditCondition="bIsStackable"))
	int32 MaxStacks = 5;

	UPROPERTY(BlueprintGetter=GetCurrentStacks, Category="Stacks")
	int32 CurrentStacks = 0;

	/**
	 * Determines how stacks will be calculated on refresh
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetStacksBehavior, Category="Stacks")
	EStatusEffectStacksRefreshBehavior StacksBehavior = EStatusEffectStacksRefreshBehavior::Ignore;

	/**
	 * Determines the amount of stacks will be added on refresh if StacksBehavior is Increase.
	 */
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetDeltaStacks,
		Category="Stacks",
		meta=(ClampMin=1, UIMin=1,
			EditCondition="bIsStackable && StacksBehavior == EStatusEffectStacksRefreshBehavior::Increase"))
	int32 DeltaStacks = 1;

	/**
	 * Holds a reference to the target actor affected by the status effect.
	 */
	UPROPERTY(BlueprintGetter=GetTargetActor, Category="StatusEffect")
	AActor* TargetActor = nullptr;

	/**
	 * Represents the actor responsible for initiating or applying the status effect.
	 */
	UPROPERTY(BlueprintGetter=GetInstigatorActor, Category="StatusEffect")
	AActor* InstigatorActor = nullptr;

	/**
	 * Holds a reference to the owning Status Effects Manager Component.
	 */
	UPROPERTY(BlueprintGetter=GetOwningManager, Category="StatusEffect")
	TObjectPtr<UStatusEffectsManagerComponent> OwningManager = nullptr;

	void ProcessTick(float DeltaTime);

	void ProcessEffectDurationTimer(float DeltaTime);

	void RefreshTimer();

	void RefreshStacks();

#if WITH_EDITOR && !UE_BUILD_SHIPPING
	static void PrintLog(const FString& Message);

	static void PrintWarning(const FString& Message);

	static void PrintError(const FString& Message);

	static void GetActorName(AActor* Actor, FString& OutName);
#endif 
};
