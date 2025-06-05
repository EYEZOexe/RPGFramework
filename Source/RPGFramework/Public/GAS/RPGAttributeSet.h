// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RPGAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * RPG Character Attributes
 */
UCLASS()
class RPGFRAMEWORK_API URPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	public:
	URPGAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes", ReplicatedUsing = OnRep_Adrenaline)
	FGameplayAttributeData Adrenaline;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Adrenaline);

	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes", ReplicatedUsing = OnRep_MaxAdrenaline)
	FGameplayAttributeData MaxAdrenaline;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxAdrenaline);
	
	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes", ReplicatedUsing = OnRep_ExperiencePoints)
	FGameplayAttributeData ExperiencePoints;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, ExperiencePoints);

	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes", ReplicatedUsing = OnRep_MaxExperiencePoints)
	FGameplayAttributeData MaxExperiencePoints;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxExperiencePoints);

protected:

	UFUNCTION()
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
	

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
	
	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
	
	UFUNCTION()
	virtual void OnRep_Adrenaline(const FGameplayAttributeData& OldAdrenaline);
	
	UFUNCTION()
	virtual void OnRep_MaxAdrenaline(const FGameplayAttributeData& OldMaxAdrenaline);

	UFUNCTION()
	virtual void OnRep_ExperiencePoints(const FGameplayAttributeData& OldExperiencePoints);

	UFUNCTION()
	virtual void OnRep_MaxExperiencePoints(const FGameplayAttributeData& OldMaxExperiencePoints);
	
};
