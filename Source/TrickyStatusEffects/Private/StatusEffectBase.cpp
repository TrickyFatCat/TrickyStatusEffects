// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "StatusEffectBase.h"

#include "StatusEffectsManagerComponent.h"

void UStatusEffectBase::Tick(float DeltaTime)
{
	TickEffect(DeltaTime);
}

bool UStatusEffectBase::IsTickable() const
{
	return !IsUnreachable() && !IsTemplate(RF_ClassDefaultObject);
}

bool UStatusEffectBase::IsTickableWhenPaused() const
{
	return !IsUnreachable();
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
		MarkAsGarbage();
	}
	
	return bResult;
}

TStatId UStatusEffectBase::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UStatusEffectBase, STATGROUP_Tickables);
}
