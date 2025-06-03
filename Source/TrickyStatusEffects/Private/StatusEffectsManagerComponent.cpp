// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "StatusEffectsManagerComponent.h"

#include "StatusEffectBase.h"


UStatusEffectsManagerComponent::UStatusEffectsManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UStatusEffectsManagerComponent::ApplyStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Instigator)
{
	if (!IsValid(StatusEffect))
	{
		return false;
	}

	UStatusEffectBase* NewStatusEffect = NewObject<UStatusEffectBase>(this, StatusEffect);

	if (!IsValid(NewStatusEffect))
	{
		return false;
	}

	const bool bIsApplied = NewStatusEffect->Activate(Instigator, GetOwner());
	
	if (bIsApplied)
	{
		StatusEffects.Add(NewStatusEffect);
		NewStatusEffect->OnStatusEffectDeactivated.AddUniqueDynamic(this, &UStatusEffectsManagerComponent::HandleStatusEffectDeactivated);
	}

	return bIsApplied;
}

bool UStatusEffectsManagerComponent::RemoveStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect, AActor* Remover)
{
	bool bIsSuccess = false;
	
	if (!HasStatusEffect(StatusEffect))
	{
		return bIsSuccess;
	}

	UStatusEffectBase* TargetStatusEffect = GetStatusEffect(StatusEffect);

	if (!IsValid(TargetStatusEffect))
	{
		return bIsSuccess;
	}
	
	bIsSuccess = TargetStatusEffect->Deactivate(Remover);

	return bIsSuccess;
}

bool UStatusEffectsManagerComponent::HasStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect) const
{
	if (StatusEffects.IsEmpty() || !IsValid(StatusEffect))
	{
		return false;
	}

	auto Predicate = [StatusEffect](const UStatusEffectBase* Effect)
	{
		return Effect->GetClass() == StatusEffect;
	};
	
	return StatusEffects.ContainsByPredicate(Predicate);
}

UStatusEffectBase* UStatusEffectsManagerComponent::GetStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect) const
{
	if (!IsValid(StatusEffect))
	{
		return nullptr;
	}

	auto Predicate = [StatusEffect](const UStatusEffectBase* Effect)
	{
		return Effect->GetClass() == StatusEffect;
	};
	
	return *StatusEffects.FindByPredicate(Predicate);
}

void UStatusEffectsManagerComponent::HandleStatusEffectDeactivated(UStatusEffectBase* StatusEffect, AActor* Deactivator)
{
	if (!IsValid(StatusEffect) || !StatusEffects.Contains(StatusEffect))
	{
		return;
	}
	
	StatusEffects.Remove(StatusEffect);
}
