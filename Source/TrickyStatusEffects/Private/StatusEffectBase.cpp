// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "StatusEffectBase.h"

#include "StatusEffectsManagerComponent.h"

void UStatusEffectBase::Tick(float DeltaTime)
{
	TickEvent(DeltaTime);
}

bool UStatusEffectBase::IsTickable() const
{
	return !IsUnreachable();
}

bool UStatusEffectBase::IsTickableWhenPaused() const
{
	return !IsUnreachable();
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
	const bool bResult = ActivationEvent();

	if (!bResult)
	{
		MarkAsGarbage();
	}

	return bResult;
}

bool UStatusEffectBase::DeactivateStatusEffect(AActor* Deactivator)
{
	if (!IsValid(this))
	{
		return false;
	}

	const bool bResult = DeactivationEvent(Deactivator);

	if (bResult)
	{
		MarkAsGarbage();
	}
	
	return bResult;
}

bool UStatusEffectBase::ActivationEvent_Implementation()
{
	return false;
}

void UStatusEffectBase::TickEvent_Implementation(float DeltaTime)
{
}

bool UStatusEffectBase::DeactivationEvent_Implementation(AActor* Deactivator)
{
	return false;
}


TStatId UStatusEffectBase::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UStatusEffectBase, STATGROUP_Tickables);
}
