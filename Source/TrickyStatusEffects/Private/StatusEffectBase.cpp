// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "StatusEffectBase.h"

#include "StatusEffectsManagerComponent.h"

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

bool UStatusEffectBase::ActivateStatusEffect(AActor* Instigator, AActor* Target)
{
	bool bIsSuccess = false;

	if (!IsValid(Target))
	{
		MarkAsGarbage();
		return bIsSuccess;
	}

	OwningManager = Target->GetComponentByClass<UStatusEffectsManagerComponent>();

	if (!IsValid(OwningManager))
	{
		MarkAsGarbage();
		return bIsSuccess;
	}

	TargetActor = Target;
	InstigatorActor = Instigator;
	bIsSuccess = ActivateEffect();

	if (!bIsSuccess)
	{
		MarkAsGarbage();
	}

	if (!bIsInfinite)
	{
		StatusEffectTimer = Duration;
	}
	
	return bIsSuccess;
}

bool UStatusEffectBase::DeactivateStatusEffect(AActor* Deactivator)
{
	if (!IsValid(this))
	{
		return false;
	}

	const bool bResult = DeactivateEffect(Deactivator);

	if (bResult)
	{
		OnStatusEffectDeactivated.Broadcast(this, Deactivator);
		MarkAsGarbage();
		OnStatusEffectDeactivated.Clear();
	}

	return bResult;
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
		DeactivateStatusEffect(nullptr);
	}
}
