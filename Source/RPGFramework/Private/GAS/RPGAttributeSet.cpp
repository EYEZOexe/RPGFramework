// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RPGAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "RPGCharacter.h"

URPGAttributeSet::URPGAttributeSet()
{
	
}

void URPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(URPGAttributeSet, Health);
	DOREPLIFETIME(URPGAttributeSet, MaxHealth);
	DOREPLIFETIME(URPGAttributeSet, Stamina);
	DOREPLIFETIME(URPGAttributeSet, MaxStamina);
	DOREPLIFETIME(URPGAttributeSet, Adrenaline);
	DOREPLIFETIME(URPGAttributeSet, MaxAdrenaline);
	DOREPLIFETIME(URPGAttributeSet, ExperiencePoints);
	DOREPLIFETIME(URPGAttributeSet, MaxExperiencePoints);
}

void URPGAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive)
	{
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	AActor* TargetActor = nullptr;
	ARPGCharacter* RPGCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		RPGCharacter = Cast<ARPGCharacter>(TargetActor);
	}
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		if (RPGCharacter)
		{
			RPGCharacter->OnHealthChanged(DeltaValue, Data.EffectSpec.GetContext().GetInstigator());
		}
	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		bool StaminaOverflow = GetStamina() > GetMaxStamina() ? FMath::IsNearlyEqual(GetStamina() - DeltaValue, GetMaxStamina()) : false;
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
		if (RPGCharacter && !StaminaOverflow)
		{
			RPGCharacter->OnStaminaChanged(DeltaValue, Data.EffectSpec.GetContext().GetInstigator());
		}
	}

	if (Data.EvaluatedData.Attribute == GetAdrenalineAttribute())
	{
		SetAdrenaline(FMath::Clamp(GetAdrenaline(), 0.0f, GetMaxAdrenaline()));
		if (RPGCharacter)
		{
			RPGCharacter->OnAdrenalineChanged(DeltaValue, Data.EffectSpec.GetContext().GetInstigator());
		}
	}

	if (Data.EvaluatedData.Attribute == GetExperiencePointsAttribute())
	{
		float Difference = GetExperiencePoints() - GetMaxExperiencePoints();
		if (RPGCharacter)
		{
			RPGCharacter->OnExperiencePointsChanged(DeltaValue);
			if (Difference >= 0)
			{
				RPGCharacter->HandleCharacterLevelUp();
			}
		}
	}
}

void URPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}

	if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
	}

	if (Attribute == GetMaxAdrenalineAttribute())
	{
		AdjustAttributeForMaxChange(Adrenaline, MaxAdrenaline, NewValue, GetAdrenalineAttribute());
	}

	if (Attribute == GetMaxExperiencePointsAttribute())
	{
		SetExperiencePoints(0.0f); // Reset experience points when max experience points change
	}
}

void URPGAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilitySystemComponent)
	{
		AbilitySystemComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Override, NewMaxValue);
	}
}

void URPGAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Health, OldHealth);
}

void URPGAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxHealth, OldMaxHealth);
}

void URPGAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Stamina, OldStamina);
}

void URPGAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxStamina, OldMaxStamina);
}

void URPGAttributeSet::OnRep_Adrenaline(const FGameplayAttributeData& OldAdrenaline)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Adrenaline, OldAdrenaline);
}

void URPGAttributeSet::OnRep_MaxAdrenaline(const FGameplayAttributeData& OldMaxAdrenaline)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxAdrenaline, OldMaxAdrenaline);
}

void URPGAttributeSet::OnRep_ExperiencePoints(const FGameplayAttributeData& OldExperiencePoints)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, ExperiencePoints, OldExperiencePoints);
}

void URPGAttributeSet::OnRep_MaxExperiencePoints(const FGameplayAttributeData& OldMaxExperiencePoints)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxExperiencePoints, OldMaxExperiencePoints);
}
