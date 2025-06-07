// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "StatusEffectsLibrary.h"

#include "StatusEffectsManagerComponent.h"
#include "StatusEffectBase.h"

UStatusEffectsManagerComponent* UStatusEffectsLibrary::GetStatusEffectsManager(AActor* Target)
{
	if (!IsValid(Target))
	{
		return nullptr;
	}

	return Target->FindComponentByClass<UStatusEffectsManagerComponent>();
}

UStatusEffectBase* UStatusEffectsLibrary::ApplyStatusEffect(AActor* Target,
                                                            const TSubclassOf<UStatusEffectBase> StatusEffect,
                                                            AActor* Instigator)
{
	if (!IsValid(Target) || !IsValid(StatusEffect))
	{
		return nullptr;
	}

	UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return nullptr;
	}

	return StatusEffectsManager->ApplyStatusEffect(StatusEffect, Instigator);
}

void UStatusEffectsLibrary::RefreshAllStatusEffects(AActor* Target)
{
	if (!IsValid(Target))
	{
		return;
	}

	UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->RefreshAllStatusEffects();
}

void UStatusEffectsLibrary::RefreshAllStatusEffectsOfClass(AActor* Target, TSubclassOf<UStatusEffectBase> StatusEffect)
{
	if (!IsValid(Target) || !IsValid(StatusEffect))
	{
		return;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->RefreshAllStatusEffectsOfClass(StatusEffect);
}

void UStatusEffectsLibrary::RefreshAllStatusEffectsFromInstigator(AActor* Target, AActor* Instigator)
{
	if (!IsValid(Target))
	{
		return;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->RefreshAllStatusEffectsFromInstigator(Instigator);
}

void UStatusEffectsLibrary::RefreshAllStatusEffectsOfClassFromInstigator(AActor* Target,
                                                                         TSubclassOf<UStatusEffectBase> StatusEffect,
                                                                         AActor* Instigator)
{
	if (!IsValid(Target) || !IsValid(StatusEffect))
	{
		return;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->RefreshAllStatusEffectsOfClassFromInstigator(StatusEffect, Instigator);
}

void UStatusEffectsLibrary::RefreshAllStatusEffectsOfType(AActor* Target, const EStatusEffectType StatusEffectType)
{
	if (!IsValid(Target))
	{
		return;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->RefreshAllStatusEffectsOfType(StatusEffectType);
}

void UStatusEffectsLibrary::RefreshAllStatusEffectsOfTypeFromInstigator(AActor* Target,
                                                                        const EStatusEffectType StatusEffectType,
                                                                        AActor* Instigator)
{
	if (!IsValid(Target))
	{
		return;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->RefreshAllStatusEffectsOfTypeFromInstigator(StatusEffectType, Instigator);
}

bool UStatusEffectsLibrary::RemoveStatusEffect(AActor* Target,
                                               const TSubclassOf<UStatusEffectBase> StatusEffect,
                                               AActor* Remover)
{
	if (!IsValid(Target) || !IsValid(StatusEffect))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return false;
	}

	return StatusEffectsManager->RemoveStatusEffect(StatusEffect, Remover);
}

bool UStatusEffectsLibrary::RemoveStatusEffectFromInstigator(AActor* Target,
                                                             const TSubclassOf<UStatusEffectBase> StatusEffect,
                                                             AActor* Instigator,
                                                             AActor* Remover)
{
	if (!IsValid(Target) || !IsValid(StatusEffect))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return false;
	}

	return StatusEffectsManager->RemoveStatusEffectFromInstigator(StatusEffect, Instigator, Remover);
}

void UStatusEffectsLibrary::RemoveAllStatusEffects(AActor* Target, AActor* Remover)
{
	if (!IsValid(Target))
	{
		return;
	}

	UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->RemoveAllStatusEffects(Remover);
}

void UStatusEffectsLibrary::RemoveAllStatusEffectsFromInstigator(AActor* Target, AActor* Instigator, AActor* Remover)
{
	if (!IsValid(Target))
	{
		return;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->RemoveAllStatusEffectsFromInstigator(Remover, Instigator);
}

void UStatusEffectsLibrary::RemoveAllStatusEffectsOfClass(AActor* Target,
                                                          const TSubclassOf<UStatusEffectBase> StatusEffect,
                                                          AActor* Remover)
{
	if (!IsValid(Target) || !IsValid(StatusEffect))
	{
		return;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->RemoveAllStatusEffectsOfClass(StatusEffect, Remover);
}

void UStatusEffectsLibrary::RemoveAllStatusEffectsOfClassFromInstigator(AActor* Target,
                                                                        TSubclassOf<UStatusEffectBase> StatusEffect,
                                                                        AActor* Instigator,
                                                                        AActor* Remover)
{
	if (!IsValid(Target) || !IsValid(StatusEffect))
	{
		return;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->RemoveAllStatusEffectsOfClassFromInstigator(StatusEffect, Instigator, Remover);
}

void UStatusEffectsLibrary::RemoveAllStatusEffectsOfType(AActor* Target,
                                                         const EStatusEffectType StatusEffectType,
                                                         AActor* Remover)
{
	if (!IsValid(Target))
	{
		return;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->RemoveAllStatusEffectsOfType(StatusEffectType, Remover);
}

void UStatusEffectsLibrary::RemoveAllStatusEffectsOfTypeFromInstigator(AActor* Target,
                                                                       EStatusEffectType StatusEffectType,
                                                                       AActor* Instigator,
                                                                       AActor* Remover)
{
	if (!IsValid(Target))
	{
		return;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->RemoveAllStatusEffectsOfTypeFromInstigator(StatusEffectType, Instigator, Remover);
}

bool UStatusEffectsLibrary::HasStatusEffect(AActor* Target, TSubclassOf<UStatusEffectBase> StatusEffect)
{
	if (!IsValid(Target) || !IsValid(StatusEffect))
	{
		return false;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return false;
	}

	return StatusEffectsManager->HasStatusEffect(StatusEffect);
}

bool UStatusEffectsLibrary::HasStatusEffectFromInstigator(AActor* Target,
                                                          TSubclassOf<UStatusEffectBase> StatusEffect,
                                                          AActor* Instigator)
{
	if (!IsValid(Target) || !IsValid(StatusEffect))
	{
		return false;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return false;
	}

	return StatusEffectsManager->HasStatusEffectFromInstigator(StatusEffect, Instigator);
}

bool UStatusEffectsLibrary::HasAnyStatusEffect(AActor* Target)
{
	if (!IsValid(Target))
	{
		return false;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return false;
	}

	return StatusEffectsManager->HasAnyStatusEffect();
}

bool UStatusEffectsLibrary::HasAnyStatusEffectFromInstigator(AActor* Target, AActor* Instigator)
{
	if (!IsValid(Target))
	{
		return false;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return false;
	}

	return StatusEffectsManager->HasAnyStatusEffectFromInstigator(Instigator);
}

bool UStatusEffectsLibrary::HasAnyStatusEffectOfType(AActor* Target, const EStatusEffectType StatusEffectType)
{
	if (!IsValid(Target))
	{
		return false;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return false;
	}

	return StatusEffectsManager->HasAnyStatusEffectOfType(StatusEffectType);
}

bool UStatusEffectsLibrary::HasAnyStatusEffectOfTypeFromInstigator(AActor* Target,
                                                                   const EStatusEffectType StatusEffectType,
                                                                   AActor* Instigator)
{
	if (!IsValid(Target))
	{
		return false;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return false;
	}

	return StatusEffectsManager->HasAnyStatusEffectOfTypeFromInstigator(StatusEffectType, Instigator);
}

UStatusEffectBase* UStatusEffectsLibrary::GetStatusEffect(AActor* Target, TSubclassOf<UStatusEffectBase> StatusEffect)
{
	if (!IsValid(Target) || !IsValid(StatusEffect))
	{
		return nullptr;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return nullptr;
	}

	return StatusEffectsManager->GetStatusEffect(StatusEffect);
}

UStatusEffectBase* UStatusEffectsLibrary::GetStatusEffectFromInstigator(AActor* Target,
                                                                        TSubclassOf<UStatusEffectBase> StatusEffect,
                                                                        AActor* Instigator)
{
	if (!IsValid(Target) || !IsValid(StatusEffect))
	{
		return nullptr;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return nullptr;
	}

	return StatusEffectsManager->GetStatusEffectFromInstigator(StatusEffect, Instigator);
}

void UStatusEffectsLibrary::GetAllStatusEffects(AActor* Target, TArray<UStatusEffectBase*>& OutStatusEffects)
{
	if (!IsValid(Target))
	{
		return;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->GetAllStatusEffects(OutStatusEffects);
}

void UStatusEffectsLibrary::GetAllStatusEffectsOfClass(AActor* Target,
                                                       TArray<UStatusEffectBase*>& OutStatusEffects,
                                                       TSubclassOf<UStatusEffectBase> StatusEffect)
{
	if (!IsValid(Target) || !IsValid(StatusEffect))
	{
		return;
	}

	UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->GetAllStatusEffectsOfClass(OutStatusEffects, StatusEffect);
}

void UStatusEffectsLibrary::GetAllStatusEffectsFromInstigator(AActor* Target,
                                                              TArray<UStatusEffectBase*>& OutStatusEffects,
                                                              AActor* Instigator)
{
	if (!IsValid(Target))
	{
		return;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->GetAllStatusEffectsFromInstigator(OutStatusEffects, Instigator);
}

void UStatusEffectsLibrary::GetAllStatusEffectsOfClassFromInstigator(AActor* Target,
                                                                     TArray<UStatusEffectBase*>& OutStatusEffects,
                                                                     TSubclassOf<UStatusEffectBase> StatusEffect,
                                                                     AActor* Instigator)
{
	if (!IsValid(Target) || !IsValid(StatusEffect))
	{
		return;
	}

	UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->GetAllStatusEffectsOfClassFromInstigator(OutStatusEffects, StatusEffect, Instigator);
}

void UStatusEffectsLibrary::GetAllStatusEffectsOfType(AActor* Target,
                                                      TArray<UStatusEffectBase*>& OutStatusEffects,
                                                      const EStatusEffectType StatusEffectType)
{
	if (!IsValid(Target))
	{
		return;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->GetAllStatusEffectsOfType(OutStatusEffects, StatusEffectType);
}

void UStatusEffectsLibrary::GetAllStatusEffectsOfTypeFromInstigator(AActor* Target,
                                                                    TArray<UStatusEffectBase*>& OutStatusEffects,
                                                                    const EStatusEffectType StatusEffectType,
                                                                    AActor* Instigator)
{
	if (!IsValid(Target))
	{
		return;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManager = GetStatusEffectsManager(Target);

	if (!IsValid(StatusEffectsManager))
	{
		return;
	}

	StatusEffectsManager->GetAllStatusEffectsOfTypeFromInstigator(OutStatusEffects, StatusEffectType, Instigator);
}
