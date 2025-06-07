// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "StatusEffectsManagerComponent.h"

#include "StatusEffectBase.h"


UStatusEffectsManagerComponent::UStatusEffectsManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatusEffectsManagerComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	RemoveAllStatusEffects(GetOwner());
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
		{
			TargetStatusEffect = CreateNewStatusEffect(StatusEffect, Instigator);
		}
		break;

	case EStatusEffectScope::PerInstigator:
		{
			if (!IsValid(Instigator))
			{
				return TargetStatusEffect;
			}

			if (HasStatusEffectFromInstigator(StatusEffect, Instigator))
			{
				TargetStatusEffect = GetStatusEffectFromInstigator(StatusEffect, Instigator);
				TargetStatusEffect->Refresh();
			}
			else
			{
				TargetStatusEffect = CreateNewStatusEffect(StatusEffect, Instigator);
			}
		}
		break;

	case EStatusEffectScope::PerTarget:
		{
			if (HasStatusEffect(StatusEffect))
			{
				TargetStatusEffect = GetStatusEffect(StatusEffect);
				TargetStatusEffect->Refresh();
			}
			else
			{
				TargetStatusEffect = CreateNewStatusEffect(StatusEffect, Instigator);
			}
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

void UStatusEffectsManagerComponent::RefreshAllStatusEffectsOfClass(
	const TSubclassOf<UStatusEffectBase> StatusEffect) const
{
	if (AppliedStatusEffects.IsEmpty() || !IsValid(StatusEffect))
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsOfClass(StatusEffects, StatusEffect);
	RefreshGivenStatusEffects(StatusEffects);
}

void UStatusEffectsManagerComponent::RefreshAllStatusEffectsFromInstigator(AActor* Instigator) const
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsFromInstigator(StatusEffects, Instigator);
	RefreshGivenStatusEffects(StatusEffects);
}

void UStatusEffectsManagerComponent::RefreshAllStatusEffectsOfClassFromInstigator(
	TSubclassOf<UStatusEffectBase> StatusEffect,
	AActor* Instigator) const
{
	if (AppliedStatusEffects.IsEmpty() || !IsValid(StatusEffect))
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsOfClassFromInstigator(StatusEffects, StatusEffect, Instigator);
	RefreshGivenStatusEffects(StatusEffects);
}

void UStatusEffectsManagerComponent::RefreshAllStatusEffectsOfType(const EStatusEffectType StatusEffectType) const
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsOfType(StatusEffects, StatusEffectType);
	RefreshGivenStatusEffects(StatusEffects);
}

void UStatusEffectsManagerComponent::RefreshAllStatusEffectsOfTypeFromInstigator(
	const EStatusEffectType StatusEffectType,
	AActor* Instigator) const
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsOfTypeFromInstigator(StatusEffects, StatusEffectType, Instigator);
	RefreshGivenStatusEffects(StatusEffects);
}

bool UStatusEffectsManagerComponent::RemoveStatusEffect(const TSubclassOf<UStatusEffectBase> StatusEffect,
                                                        AActor* Remover) 
{
	if (!HasStatusEffect(StatusEffect))
	{
		return false;
	}

	UStatusEffectBase* TargetStatusEffect = GetStatusEffect(StatusEffect);

	if (!IsValid(TargetStatusEffect))
	{
		return false;
	}

	TargetStatusEffect->Deactivate(Remover);
	return true;
}

bool UStatusEffectsManagerComponent::RemoveStatusEffectFromInstigator(TSubclassOf<UStatusEffectBase> StatusEffect,
                                                                      AActor* Instigator,
                                                                      AActor* Remover)
{
	if (!HasStatusEffectFromInstigator(StatusEffect, Instigator))
	{
		return false;
	}

	UStatusEffectBase* TargetStatusEffect = GetStatusEffectFromInstigator(StatusEffect, Instigator);

	if (!IsValid(TargetStatusEffect))
	{
		return false;
	}

	TargetStatusEffect->Deactivate(Remover);
	return true;
}

void UStatusEffectsManagerComponent::RemoveAllStatusEffects(AActor* Remover)
{
	RemoveGivenStatusEffects(AppliedStatusEffects, Remover);
}

void UStatusEffectsManagerComponent::RemoveAllStatusEffectsFromInstigator(AActor* Remover, AActor* Instigator) const
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsFromInstigator(StatusEffects, Instigator);
	RemoveGivenStatusEffects(StatusEffects, Remover);
}

void UStatusEffectsManagerComponent::RemoveAllStatusEffectsOfClass(const TSubclassOf<UStatusEffectBase> StatusEffect,
                                                                   AActor* Remover) const
{
	if (AppliedStatusEffects.IsEmpty() || !IsValid(StatusEffect))
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsOfClass(StatusEffects, StatusEffect);
	RemoveGivenStatusEffects(StatusEffects, Remover);
}

void UStatusEffectsManagerComponent::RemoveAllStatusEffectsOfClassFromInstigator(
	const TSubclassOf<UStatusEffectBase> StatusEffect,
	AActor* Instigator,
	AActor* Remover) const
{
	if (AppliedStatusEffects.IsEmpty() || !IsValid(StatusEffect))
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsOfClassFromInstigator(StatusEffects, StatusEffect, Instigator);
	RemoveGivenStatusEffects(StatusEffects, Remover);
}

void UStatusEffectsManagerComponent::RemoveAllStatusEffectsOfType(const EStatusEffectType StatusEffectType,
                                                                  AActor* Remover) const
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsOfType(StatusEffects, StatusEffectType);
	RemoveGivenStatusEffects(StatusEffects, Remover);
}

void UStatusEffectsManagerComponent::RemoveAllStatusEffectsOfTypeFromInstigator(
	const EStatusEffectType StatusEffectType,
	AActor* Instigator,
	AActor* Remover) const
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return;
	}

	TArray<UStatusEffectBase*> StatusEffects;
	GetAllStatusEffectsOfTypeFromInstigator(StatusEffects, StatusEffectType, Instigator);
	RemoveGivenStatusEffects(StatusEffects, Remover);
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

bool UStatusEffectsManagerComponent::HasStatusEffectFromInstigator(TSubclassOf<UStatusEffectBase> StatusEffect,
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

bool UStatusEffectsManagerComponent::HasAnyStatusEffect() const
{
	return AppliedStatusEffects.Num() > 0;
}

bool UStatusEffectsManagerComponent::HasAnyStatusEffectFromInstigator(AActor* Instigator) const
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

bool UStatusEffectsManagerComponent::HasAnyStatusEffectOfType(const EStatusEffectType StatusEffectType) const
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return false;
	}

	auto Predicate = [StatusEffectType](const UStatusEffectBase* Effect)
	{
		return Effect->GetEffectType() == StatusEffectType;
	};

	return AppliedStatusEffects.ContainsByPredicate(Predicate);
}

bool UStatusEffectsManagerComponent::HasAnyStatusEffectOfTypeFromInstigator(const EStatusEffectType StatusEffectType,
                                                                            AActor* Instigator) const
{
	if (AppliedStatusEffects.IsEmpty())
	{
		return false;
	}

	auto Predicate = [StatusEffectType, Instigator](const UStatusEffectBase* Effect)
	{
		return Effect->GetEffectType() == StatusEffectType && Effect->GetInstigatorActor() == Instigator;
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

UStatusEffectBase* UStatusEffectsManagerComponent::GetStatusEffectFromInstigator(
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

void UStatusEffectsManagerComponent::GetAllStatusEffectsOfClass(TArray<UStatusEffectBase*>& OutStatusEffects,
                                                                TSubclassOf<UStatusEffectBase> StatusEffect) const
{
	if (AppliedStatusEffects.IsEmpty() || !IsValid(StatusEffect))
	{
		return;
	}

	auto Predicate = [StatusEffect](const UStatusEffectBase* Effect)
	{
		return Effect->GetClass() == StatusEffect;
	};

	OutStatusEffects = AppliedStatusEffects.FilterByPredicate(Predicate);
}

void UStatusEffectsManagerComponent::GetAllStatusEffectsFromInstigator(TArray<UStatusEffectBase*>& OutStatusEffects,
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

void UStatusEffectsManagerComponent::GetAllStatusEffectsOfClassFromInstigator(
	TArray<UStatusEffectBase*>& OutStatusEffects,
	TSubclassOf<UStatusEffectBase> StatusEffect,
	AActor* Instigator) const
{
	if (AppliedStatusEffects.IsEmpty() || !IsValid(StatusEffect))
	{
		return;
	}

	auto Predicate = [StatusEffect, Instigator](const UStatusEffectBase* Effect)
	{
		return Effect->GetInstigatorActor() == Instigator && Effect->GetClass() == StatusEffect;
	};

	OutStatusEffects = AppliedStatusEffects.FilterByPredicate(Predicate);
}

void UStatusEffectsManagerComponent::GetAllStatusEffectsOfType(TArray<UStatusEffectBase*>& OutStatusEffects,
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

void UStatusEffectsManagerComponent::GetAllStatusEffectsOfTypeFromInstigator(
	TArray<UStatusEffectBase*>& OutStatusEffects,
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

	OnStatusEffectRemoved.Broadcast(this, StatusEffect, Deactivator);
	AppliedStatusEffects.Remove(StatusEffect);
}

void UStatusEffectsManagerComponent::HandleStatusEffectRefreshed(UStatusEffectBase* StatusEffect)
{
	if (!IsValid(StatusEffect) || !AppliedStatusEffects.Contains(StatusEffect))
	{
		return;
	}

	OnStatusEffectRefreshed.Broadcast(this, StatusEffect);
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

	const bool bIsApplied = NewStatusEffect->Activate(this, Instigator);

	if (!bIsApplied)
	{
		return nullptr;
	}

	AppliedStatusEffects.Add(NewStatusEffect);
	NewStatusEffect->OnStatusEffectDeactivated.AddUniqueDynamic(
		this, &UStatusEffectsManagerComponent::HandleStatusEffectDeactivated);
	NewStatusEffect->OnStatusEffectRefreshed.AddUniqueDynamic(
		this, &UStatusEffectsManagerComponent::HandleStatusEffectRefreshed);
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

void UStatusEffectsManagerComponent::RemoveGivenStatusEffects(TArray<UStatusEffectBase*>& StatusEffects,
                                                              AActor* Remover)
{
	if (StatusEffects.IsEmpty())
	{
		return;
	}

	for (UStatusEffectBase* StatusEffect : StatusEffects)
	{
		StatusEffect->Deactivate(Remover);
	}
}
