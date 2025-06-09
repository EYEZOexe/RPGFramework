// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GAS/RPGAttributeSet.h"
#include "GameFramework/Character.h"
#include "RPGCharacter.generated.h"

UCLASS()
class RPGFRAMEWORK_API ARPGCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacter();

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual int32 GetCharacterLevel() const { return CharacterLevel; }

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetHealth() const { return AttributeSet ? AttributeSet->GetHealth() : 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetMaxHealth() const { return AttributeSet ? AttributeSet->GetMaxHealth() : 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetStamina() const { return AttributeSet ? AttributeSet->GetStamina() : 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetMaxStamina() const { return AttributeSet ? AttributeSet->GetMaxStamina() : 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetAdrenaline() const { return AttributeSet ? AttributeSet->GetAdrenaline() : 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetMaxAdrenaline() const { return AttributeSet ? AttributeSet->GetMaxAdrenaline() : 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetExperiencePoints() const { return AttributeSet ? AttributeSet->GetExperiencePoints() : 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetMaxExperiencePoints() const { return AttributeSet ? AttributeSet->GetMaxExperiencePoints() : 0.0f; }

	// Try to activate all gameplay abilities that match tag.
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	bool ActivateAbilitiesWithTag(FGameplayTagContainer AbilityTags, bool AllowRemoteActivation = true);

	/* Called when health is changed  
	 * @param DeltaValue - will be negative for health decrease, positive otherwise.
	 * @param Instigator - the actor that caused the health change.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, AActor* Causer);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStaminaChanged(float DeltaValue, AActor* Causer);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAdrenalineChanged(float DeltaValue, AActor* Causer);

	UFUNCTION(BlueprintImplementableEvent)
	void OnExperiencePointsChanged(float DeltaValue);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterLevelUp();

	UFUNCTION(BlueprintCallable, Category = "RPG Abilities|Melee")
	bool ActivateMeleeAbility(bool AllowRemoteActivation = true);

	UFUNCTION(BlueprintCallable, Category = "RPG Abilities")
	virtual void GetActiveAbilitiesWithTag(FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities, bool MatchExactTag);

	UFUNCTION(BlueprintCallable, Category = "RPG Abilities")
	virtual void ApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect);

	UFUNCTION(BlueprintCallable, Category = "RPG Abilities")
	virtual bool CanApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect);
	
	/*
	 * Called when the character runs out of health.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnDead();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class URPGAttributeSet* AttributeSet;

	// Character Level (it should not be changed directly once character is spawned!)
	UPROPERTY(EditAnywhere, Category = "RPG Attributes")
	int32 CharacterLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Attributes")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultAttributeEffects;

	// Set this to true to enable test abilities for debugging purposes
	UPROPERTY(EditAnywhere, Category = "RPG Attributes|Debug")
	bool EnableTestAbilities;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Attributes|Debug")
	TArray<TSubclassOf<class UGameplayAbility>> TestAbilities;

	virtual void SetTestAbilities();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Abilities|Melee")
	TSubclassOf<class UGameplayAbility> MeleeAbility;

	UPROPERTY()
	FGameplayAbilitySpecHandle MeleeAbilitySpecHandle;

	virtual void SetMeleeAbility();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

	virtual void PossessedBy(AController* NewController) override;
	
	//implement ability system interface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void HandleHealthChange(float DeltaValue, AActor* Causer);

	virtual void HandleStaminaChange(float DeltaValue, AActor* Causer);

	virtual void HandleAdrenalineChange(float DeltaValue, AActor* Causer);

	virtual void HandleExperiencePointsChange(float DeltaValue);

	virtual void HandleCharacterLevelUp();

	virtual void ApplyDefaultAttributeEffects();

	virtual void RemoveDefaultAttributeEffects();
};


