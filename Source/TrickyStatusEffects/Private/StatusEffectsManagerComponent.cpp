// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "StatusEffectsManagerComponent.h"

#include "StatusEffectBase.h"


UStatusEffectsManagerComponent::UStatusEffectsManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UStatusEffectBase* UStatusEffectsManagerComponent::ApplyStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect,
                                                                     AActor* Instigator)
{
	if (!IsValid(StatusEffect))
	{
		return nullptr;
	}

	const UStatusEffectBase* StatusEffectCDO = StatusEffect->GetDefaultObject<UStatusEffectBase>();
	UStatusEffectBase* TargetStatusEffect = nullptr;

	switch (StatusEffectCDO->GetEffectScope())
	{
	case EStatusEffectScope::PerInstance:
		TargetStatusEffect = CreateNewStatusEffect(StatusEffect, Instigator);
		break;

	case EStatusEffectScope::PerInstigator:
		if (!IsValid(Instigator))
		{
			return TargetStatusEffect;
		}
		
		if (HasStatusEffectByInstigator(StatusEffect, Instigator))
		{
			TargetStatusEffect = GetStatusEffectByInstigator(StatusEffect, Instigator);
			TargetStatusEffect->Refresh();
		}
		else
		{
			TargetStatusEffect = CreateNewStatusEffect(StatusEffect, Instigator);
		}
		break;

	case EStatusEffectScope::PerTarget:
		if (HasStatusEffect(StatusEffect))
		{
			TargetStatusEffect = GetStatusEffect(StatusEffect);
			TargetStatusEffect->Refresh();
		}
		else
		{
			TargetStatusEffect = CreateNewStatusEffect(StatusEffect, Instigator);
		}
		break;
	}

	return TargetStatusEffect;
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

bool UStatusEffectsManagerComponent::HasStatusEffectByInstigator(TSubclassOf<UStatusEffectBase> StatusEffect,
                                                                 AActor* Instigator) const
{
	if (!IsValid(StatusEffect))
	{
		return false;
	}

	auto Predicate = [StatusEffect, Instigator](const UStatusEffectBase* Effect)
	{
		return Effect->GetInstigatorActor() == Instigator && Effect->GetClass() == StatusEffect;
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

UStatusEffectBase* UStatusEffectsManagerComponent::GetStatusEffectByInstigator(
	TSubclassOf<UStatusEffectBase> StatusEffect,
	AActor* Instigator) const
{
	if (!IsValid(StatusEffect))
	{
		return nullptr;
	}

	auto Predicate = [StatusEffect, Instigator](const UStatusEffectBase* Effect)
	{
		return Effect->GetInstigatorActor() == Instigator && Effect->GetClass() == StatusEffect;
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

UStatusEffectBase* UStatusEffectsManagerComponent::CreateNewStatusEffect(const TSubclassOf<UStatusEffectBase>& StatusEffect,
                                                                         AActor* Instigator)
{
	if (!IsValid(StatusEffect))
	{
		return nullptr;
	}

	UStatusEffectBase* NewStatusEffect = NewObject<UStatusEffectBase>(this, StatusEffect);

	if (!IsValid(NewStatusEffect))
	{
		return nullptr;
	}

	const bool bIsApplied = NewStatusEffect->Activate(Instigator, GetOwner());

	if (!bIsApplied)
	{
		return nullptr;
	}

	StatusEffects.Add(NewStatusEffect);
	NewStatusEffect->OnStatusEffectDeactivated.AddUniqueDynamic(
		this, &UStatusEffectsManagerComponent::HandleStatusEffectDeactivated);
	return NewStatusEffect;
}
