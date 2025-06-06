// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "StatusEffectBase.h"
#include "GameFramework/Actor.h"

#include "StatusEffectsManagerComponent.h"

bool UStatusEffectBase::IncreaseStacks(const int32 Amount)
{
	if (!bIsStackable || Amount <= 0 || CurrentStacks >= MaxStacks)
	{
		return false;
	}

	CurrentStacks += Amount;
	CurrentStacks = FMath::Min(CurrentStacks, MaxStacks);
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
	HandleStacksDecreased(Amount);
	OnStatusEffectStacksDecreased.Broadcast(this, CurrentStacks);

	if (CurrentStacks == 0)
	{
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
	return !IsUnreachable() && !IsTemplate(RF_ClassDefaultObject);
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

bool UStatusEffectBase::Activate(AActor* Target, AActor* Instigator)
{
	if (!IsValid(Target))
	{
		MarkAsGarbage();
		return false;
	}

	OwningManager = Target->GetComponentByClass<UStatusEffectsManagerComponent>();

	if (!IsValid(OwningManager))
	{
		MarkAsGarbage();
		return false;
	}

	TargetActor = Target;
	InstigatorActor = Instigator;

	if (!CanBeActivated())
	{
		MarkAsGarbage();
		return false;
	}

	ActivateEffect();

	if (!bIsInfinite)
	{
		StatusEffectTimer = Duration;
	}

	if (bIsStackable)
	{
		CurrentStacks = InitialStacks;
	}

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
	OnStatusEffectDeactivated.Broadcast(this, Deactivator);
	MarkAsGarbage();
	OnStatusEffectDeactivated.Clear();
}

float UStatusEffectBase::GetRemainingTime() const
{
	return bIsInfinite ? -1.f : StatusEffectTimer;
}

float UStatusEffectBase::GetElapsedTime() const
{
	return bIsInfinite ? -1.f : FMath::Max(0.f, Duration - StatusEffectTimer);
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
	if (bIsInfinite || Duration <= 0.f)
	{
		return;
	}

	StatusEffectTimer -= DeltaTime;

	if (StatusEffectTimer <= 0.f)
	{
		Deactivate(nullptr);
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
		break;

	case EStatusEffectTimerRefreshBehavior::Restart:
		StatusEffectTimer = Duration;
		break;

	case EStatusEffectTimerRefreshBehavior::Extend:
		StatusEffectTimer += DeltaDuration;
		StatusEffectTimer = FMath::Min(StatusEffectTimer, MaxDuration);
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
		break;

	case EStatusEffectStacksRefreshBehavior::Reset:
		CurrentStacks = InitialStacks;
		break;

	case EStatusEffectStacksRefreshBehavior::Increase:
		IncreaseStacks(DeltaStacks);
		break;
	}
}
