// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "StatusEffectBase.h"
#include "GameFramework/Actor.h"
#include "StatusEffectsManagerComponent.h"

DEFINE_LOG_CATEGORY(LogStatusEffect)

bool UStatusEffectBase::IncreaseStacks(const int32 Amount)
{
	if (!bIsStackable || Amount <= 0 || CurrentStacks >= MaxStacks)
	{
		return false;
	}

	CurrentStacks += Amount;
	CurrentStacks = FMath::Min(CurrentStacks, MaxStacks);

#if WITH_EDITOR && !UE_BUILD_SHIPPING
	const FString Message = FString::Printf(
		TEXT("%s stacks increased by %d. Current stacks: %d"), *GetName(), Amount, CurrentStacks);
	PrintLog(Message);
#endif

	HandleStacksIncreased(Amount);
	OnStatusEffectStacksIncreased.Broadcast(this, CurrentStacks);
	return true;
}

bool UStatusEffectBase::DecreaseStacks(const int32 Amount)
{
	if (bIsStackable || Amount <= 0)
	{
		return false;
	}

	CurrentStacks -= Amount;
	CurrentStacks = FMath::Max(CurrentStacks, 0);

#if WITH_EDITOR && !UE_BUILD_SHIPPING
	FString Message = FString::Printf(
		TEXT("%s stacks decreased by %d. Current stacks: %d"), *GetName(), Amount, CurrentStacks);
	PrintLog(Message);
#endif

	HandleStacksDecreased(Amount);
	OnStatusEffectStacksDecreased.Broadcast(this, CurrentStacks);

	if (CurrentStacks == 0)
	{
#if WITH_EDITOR && !UE_BUILD_SHIPPING
		Message = FString::Printf(TEXT("%s stacks reached 0. Deactivation initiated"), *GetName());
		PrintLog(Message);
#endif

		Deactivate(nullptr);
	}

	return true;
}

void UStatusEffectBase::Tick(float DeltaTime)
{
	if (LastFrameNumberWeTicked == GFrameCounter)
	{
		return;
	}

	ProcessEffectDurationTimer(DeltaTime);
	ProcessTick(DeltaTime);
	LastFrameNumberWeTicked = GFrameCounter;
}

bool UStatusEffectBase::IsTickable() const
{
	const bool bCanEverTick = bTickEffect || !bIsInfinite && Duration > 0.f;
	return bCanEverTick && !IsUnreachable() && !IsTemplate(RF_ClassDefaultObject) && IsValid(this);
}

bool UStatusEffectBase::IsTickableWhenPaused() const
{
	return false;
}

bool UStatusEffectBase::IsTickableInEditor() const
{
	return false;
}

ETickableTickType UStatusEffectBase::GetTickableTickType() const
{
	return ETickableTickType::Conditional;
}

UWorld* UStatusEffectBase::GetTickableGameObjectWorld() const
{
	if (!IsValid(GetOuter()))
	{
		return nullptr;
	}

	return GetOuter()->GetWorld();
}

#if WITH_ENGINE
UWorld* UStatusEffectBase::GetWorld() const
{
	if (!IsValid(GetOuter()))
	{
		return nullptr;
	}

	UWorld* LocalWorld = Cast<UWorld>(GetOuter());

	if (IsValid(LocalWorld))
	{
		return LocalWorld;
	}

	return GetOuter()->GetWorld();
}

#if WITH_EDITOR
bool UStatusEffectBase::ImplementsGetWorld() const
{
	return true;
}
#endif
#endif

bool UStatusEffectBase::Activate(UStatusEffectsManagerComponent* TargetManagerComponent, AActor* Instigator)
{
	if (!IsValid(TargetManagerComponent) || !IsValid(TargetManagerComponent->GetOwner()))
	{
#if WITH_EDITOR && !UE_BUILD_SHIPPING
		const FString Message = FString::Printf(
			TEXT("%s activation failed. Target is invalid. Outer: %s"), *GetName(), *GetOuter()->GetName());
		PrintError(Message);
#endif

		MarkAsGarbage();
		return false;
	}

	OwningManager = TargetManagerComponent;
	TargetActor = TargetManagerComponent->GetOwner();
	InstigatorActor = Instigator;

	if (!CanBeActivated())
	{
#if WITH_EDITOR && !UE_BUILD_SHIPPING
		const FString Message = FString::Printf(
			TEXT("%s activation failed. CanBeActivated() check failed."), *GetName());
		PrintLog(Message);
#endif

		MarkAsGarbage();
		return false;
	}

	ActivateEffect();

	if (!bIsInfinite)
	{
		RemainingDuration = Duration;
	}

	if (bIsStackable)
	{
		CurrentStacks = InitialStacks;
	}

#if WITH_EDITOR && !UE_BUILD_SHIPPING
	FString TargetName, InstigatorName;
	GetActorName(TargetActor, TargetName);
	GetActorName(InstigatorActor, InstigatorName);
	const FString Message = FString::Printf(
		TEXT("%s activation success. Target: %s | Instigator: %s | Duration: %.2f sec | Stacks: %d"),
		*GetName(),
		*TargetName,
		*InstigatorName,
		Duration,
		CurrentStacks);
	PrintLog(Message);
#endif
	return true;
}

void UStatusEffectBase::Refresh()
{
	RefreshTimer();
	RefreshStacks();
	RefreshEffect();
	OnStatusEffectRefreshed.Broadcast(this);
}

void UStatusEffectBase::Deactivate(AActor* Deactivator)
{
	if (!IsValid(this))
	{
		return;
	}

	DeactivateEffect(Deactivator);

#if WITH_EDITOR && !UE_BUILD_SHIPPING
	FString TargetName, InstigatorName, DeactivatorName;
	GetActorName(TargetActor, TargetName);
	GetActorName(InstigatorActor, InstigatorName);
	GetActorName(Deactivator, DeactivatorName);
	const FString Message = FString::Printf(
		TEXT("%s deactivated by %s. Target: %s | Instigator: %s | RemainingTime: %.2f sec | RemainingStacks: %d"),
		*GetName(),
		*DeactivatorName,
		*TargetName,
		*InstigatorName,
		GetRemainingTime(),
		CurrentStacks);
	PrintLog(Message);
#endif

	OnStatusEffectDeactivated.Broadcast(this, Deactivator);
	MarkAsGarbage();
	OnStatusEffectDeactivated.Clear();
}

float UStatusEffectBase::GetRemainingTime() const
{
	return bIsInfinite ? -1.f : RemainingDuration;
}

float UStatusEffectBase::GetElapsedTime() const
{
	if (bIsInfinite)
	{
		return -1.f;
	}

	if (TimerBehavior == EStatusEffectTimerRefreshBehavior::Extend)
	{
		return FMath::Max(0.f, MaxDuration - RemainingDuration);
	}

	return FMath::Max(0.f, Duration - RemainingDuration);
}

TStatId UStatusEffectBase::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UStatusEffectBase, STATGROUP_Tickables);
}

void UStatusEffectBase::ProcessTick(float DeltaTime)
{
	if (!bTickEffect)
	{
		return;
	}

	if (TickInterval <= 0.f)
	{
		TickEffect(DeltaTime);
		return;
	}

	if (TickDuration > 0.f)
	{
		TickDuration -= DeltaTime;
	}
	else
	{
		TickDuration += TickInterval;
		TickEffect(TickInterval);
	}
}

void UStatusEffectBase::ProcessEffectDurationTimer(float DeltaTime)
{
	if (bIsInfinite || Duration <= 0.f || RemainingDuration <= 0.f)
	{
		return;
	}

	RemainingDuration -= DeltaTime;

	if (RemainingDuration <= 0.f)
	{
		Deactivate(nullptr);
		RemainingDuration = -1.f;
	}
}

void UStatusEffectBase::RefreshTimer()
{
	if (bIsInfinite)
	{
		return;
	}

	switch (TimerBehavior)
	{
	case EStatusEffectTimerRefreshBehavior::Ignore:
		{
		}
		break;

	case EStatusEffectTimerRefreshBehavior::Reset:
		{
			RemainingDuration = Duration;

#if WITH_EDITOR && !UE_BUILD_SHIPPING
			const FString Message = FString::Printf(TEXT("%s timer refreshed. Remaining time: %.2f sec"),
			                                        *GetName(),
			                                        RemainingDuration);
			PrintLog(Message);
#endif
		}
		break;

	case EStatusEffectTimerRefreshBehavior::Extend:
		{
			RemainingDuration += DeltaDuration;
			RemainingDuration = FMath::Min(RemainingDuration, MaxDuration);

#if WITH_EDITOR && !UE_BUILD_SHIPPING
			const FString Message = FString::Printf(TEXT("%s timer extended by %.2f sec. Remaining time: %.2f sec"),
			                                        *GetName(),
			                                        DeltaDuration,
			                                        RemainingDuration);
			PrintLog(Message);
#endif
		}
		break;
	}
}

void UStatusEffectBase::RefreshStacks()
{
	if (!bIsStackable)
	{
		return;
	}

	switch (StacksBehavior)
	{
	case EStatusEffectStacksRefreshBehavior::Ignore:
		{
		}
		break;

	case EStatusEffectStacksRefreshBehavior::Reset:
		{
			CurrentStacks = InitialStacks;

#if WITH_EDITOR && !UE_BUILD_SHIPPING
			const FString Message = FString::Printf(
				TEXT("%s stacks reset. Remaining stacks: %d"), *GetName(), CurrentStacks);
			PrintLog(Message);
#endif
		}
		break;

	case EStatusEffectStacksRefreshBehavior::Increase:
		{
			IncreaseStacks(DeltaStacks);
		}
		break;
	}
}

#if WITH_EDITOR && !UE_BUILD_SHIPPING
void UStatusEffectBase::PrintLog(const FString& Message)
{
	UE_LOG(LogStatusEffect, Log, TEXT("%s"), *Message);
}

void UStatusEffectBase::PrintWarning(const FString& Message)
{
	UE_LOG(LogStatusEffect, Warning, TEXT("%s"), *Message);
}

void UStatusEffectBase::PrintError(const FString& Message)
{
	UE_LOG(LogStatusEffect, Error, TEXT("%s"), *Message);
}

void UStatusEffectBase::GetActorName(AActor* Actor, FString& OutName)
{
	OutName = IsValid(Actor) ? Actor->GetActorNameOrLabel() : TEXT("NULL");
}
#endif
