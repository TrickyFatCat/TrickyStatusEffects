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

void UStatusEffectsManagerComponent::RefreshAllStatusEffects()
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	RefreshGivenStatusEffects(AppliedStatusEffects);
}

void UStatusEffectsManagerComponent::RefreshAllStatusEffectsByClass(TSubclassOf<UStatusEffectBase> StatusEffect) const
{
	if (AppliedStatusEffects.IsEmpty() || !IsValid(StatusEffect))
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsByClass(StatusEffects, StatusEffect);
	RefreshGivenStatusEffects(StatusEffects);
}

void UStatusEffectsManagerComponent::RefreshAllStatusEffectsByInstigator(AActor* Instigator) const
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsByInstigator(StatusEffects, Instigator);
	RefreshGivenStatusEffects(StatusEffects);
}

void UStatusEffectsManagerComponent::RefreshAllStatusEffectsByClassOfInstigator(
	TSubclassOf<UStatusEffectBase> StatusEffect,
	AActor* Instigator) const
{
	if (AppliedStatusEffects.IsEmpty() || !IsValid(StatusEffect))
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsByClassOfInstigator(StatusEffects, StatusEffect, Instigator);
	RefreshGivenStatusEffects(StatusEffects);
}

void UStatusEffectsManagerComponent::RefreshAllStatusEffectsByType(EStatusEffectType StatusEffectType)
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsByType(StatusEffects, StatusEffectType);
	RefreshGivenStatusEffects(StatusEffects);
}

void UStatusEffectsManagerComponent::RefreshAllStatusEffectsByTypeOfInstigator(EStatusEffectType StatusEffectType,
                                                                               AActor* Instigator)
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsByTypeOfInstigator(StatusEffects, StatusEffectType, Instigator);
	RefreshGivenStatusEffects(StatusEffects);
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

bool UStatusEffectsManagerComponent::RemoveStatusEffectByInstigator(TSubclassOf<UStatusEffectBase> StatusEffect,
                                                                    AActor* Remover, AActor* Instigator)
{
	return false;
}

void UStatusEffectsManagerComponent::RemoveAllStatusEffects(AActor* Remover)
{
}

void UStatusEffectsManagerComponent::RemoveAllStatusEffectsByInstigator(AActor* Remover, AActor* Instigator)
{
}

bool UStatusEffectsManagerComponent::RemoveAllStatusEffectsByClass(AActor* Remover,
                                                                   TSubclassOf<UStatusEffectBase> StatusEffect)
{
}

void UStatusEffectsManagerComponent::RemoveAllStatusEffectsByClassOfInstigator(AActor* Remover,
                                                                               TSubclassOf<UStatusEffectBase>
                                                                               StatusEffect, AActor* Instigator)
{
}

void UStatusEffectsManagerComponent::RemoveAllStatusEffectsByType(AActor* Remover, EStatusEffectType StatusEffectType)
{
}

void UStatusEffectsManagerComponent::RemoveAllStatusEffectsByTypeOfInstigator(AActor* Remover,
                                                                              EStatusEffectType StatusEffectType,
                                                                              AActor* Instigator)
{
}

bool UStatusEffectsManagerComponent::HasStatusEffect(TSubclassOf<UStatusEffectBase> StatusEffect) const
{
	if (AppliedStatusEffects.IsEmpty() || !IsValid(StatusEffect))
	{
		return false;
	}

	auto Predicate = [StatusEffect](const UStatusEffectBase* Effect)
	{
		return Effect->GetClass() == StatusEffect;
	};

	return AppliedStatusEffects.ContainsByPredicate(Predicate);
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

	return AppliedStatusEffects.ContainsByPredicate(Predicate);
}

bool UStatusEffectsManagerComponent::HasAnyStatusEffects() const
{
	return !AppliedStatusEffects.Num > 0;
}

bool UStatusEffectsManagerComponent::HasAnyStatusEffectOfInstigator(AActor* Instigator) const
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return false;
	}

	auto Predicate = [Instigator](const UStatusEffectBase* Effect)
	{
		return Effect->GetInstigatorActor() == Instigator;
	};

	return AppliedStatusEffects.ContainsByPredicate(Predicate);
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

	return *AppliedStatusEffects.FindByPredicate(Predicate);
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

	return *AppliedStatusEffects.FindByPredicate(Predicate);
}


void UStatusEffectsManagerComponent::GetAllStatusEffects(TArray<UStatusEffectBase*>& OutStatusEffects) const
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	OutStatusEffects = AppliedStatusEffects;
}

void UStatusEffectsManagerComponent::GetAllStatusEffectsByClass(TArray<UStatusEffectBase*>& OutStatusEffects,
                                                                TSubclassOf<UStatusEffectBase> StatusEffects) const
{
	if (AppliedStatusEffects.IsEmpty() || !IsValid(StatusEffects))
	{
		return;
	}

	auto Predicate = [StatusEffects](const UStatusEffectBase* Effect)
	{
		return Effect->GetClass() == StatusEffects;
	};

	OutStatusEffects = AppliedStatusEffects.FilterByPredicate(Predicate);
}

void UStatusEffectsManagerComponent::GetAllStatusEffectsByInstigator(TArray<UStatusEffectBase*>& OutStatusEffects,
                                                                     AActor* Instigator) const
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	auto Predicate = [Instigator](const UStatusEffectBase* Effect)
	{
		return Effect->GetInstigatorActor() == Instigator;
	};

	OutStatusEffects = AppliedStatusEffects.FilterByPredicate(Predicate);
}

void UStatusEffectsManagerComponent::GetAllStatusEffectsByClassOfInstigator(
	TArray<UStatusEffectBase*>& OutStatusEffects,
	TSubclassOf<UStatusEffectBase> StatusEffects,
	AActor* Instigator) const
{
	if (AppliedStatusEffects.IsEmpty() || !IsValid(StatusEffects))
	{
		return;
	}

	auto Predicate = [StatusEffects, Instigator](const UStatusEffectBase* Effect)
	{
		return Effect->GetInstigatorActor() == Instigator && Effect->GetClass() == StatusEffects;
	};

	OutStatusEffects = AppliedStatusEffects.FilterByPredicate(Predicate);
}

void UStatusEffectsManagerComponent::GetAllStatusEffectsByType(TArray<UStatusEffectBase*>& OutStatusEffects,
                                                               EStatusEffectType StatusEffectType) const
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	auto Predicate = [StatusEffectType](const UStatusEffectBase* Effect)
	{
		return Effect->GetEffectType() == StatusEffectType;
	};

	OutStatusEffects = AppliedStatusEffects.FilterByPredicate(Predicate);
}

void UStatusEffectsManagerComponent::GetAllStatusEffectsByTypeOfInstigator(TArray<UStatusEffectBase*>& OutStatusEffects,
                                                                           EStatusEffectType StatusEffectType,
                                                                           AActor* Instigator) const
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	auto Predicate = [StatusEffectType, Instigator](const UStatusEffectBase* Effect)
	{
		return Effect->GetEffectType() == StatusEffectType && Effect->GetInstigatorActor() == Instigator;
	};

	OutStatusEffects = AppliedStatusEffects.FilterByPredicate(Predicate);
}

void UStatusEffectsManagerComponent::HandleStatusEffectDeactivated(UStatusEffectBase* StatusEffect, AActor* Deactivator)
{
	if (!IsValid(StatusEffect) || !AppliedStatusEffects.Contains(StatusEffect))
	{
		return;
	}

	AppliedStatusEffects.Remove(StatusEffect);
}

UStatusEffectBase* UStatusEffectsManagerComponent::CreateNewStatusEffect(
	const TSubclassOf<UStatusEffectBase>& StatusEffect,
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

	AppliedStatusEffects.Add(NewStatusEffect);
	NewStatusEffect->OnStatusEffectDeactivated.AddUniqueDynamic(
		this, &UStatusEffectsManagerComponent::HandleStatusEffectDeactivated);
	return NewStatusEffect;
}

void UStatusEffectsManagerComponent::RefreshGivenStatusEffects(TArray<UStatusEffectBase*>& StatusEffects)
{
	if (StatusEffects.IsEmpty())
	{
		return;
	}

	for (UStatusEffectBase* StatusEffect : StatusEffects)
	{
		StatusEffect->Refresh();
	}
}
