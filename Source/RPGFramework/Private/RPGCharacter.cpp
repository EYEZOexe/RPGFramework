// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacter.h"
#include "GAS/RPGAbilitySystemComponent.h"

// Sets default values
ARPGCharacter::ARPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and initialize the Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("AttributeSet"));

	// Set default character level
	CharacterLevel = 1;
}

bool ARPGCharacter::ActivateAbilitiesWithTag(FGameplayTagContainer AbilityTags, bool AllowRemoteActivation)
{
	if (!AbilitySystemComponent) return false;

	return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, AllowRemoteActivation);
}

bool ARPGCharacter::ActivateMeleeAbility(bool AllowRemoteActivation)
{
	if (!AbilitySystemComponent || !MeleeAbilitySpecHandle.IsValid()) return false;

	return AbilitySystemComponent->TryActivateAbility(MeleeAbilitySpecHandle);
}

// Called when the game starts or when spawned
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
void ARPGCharacter::SetTestAbilities()
{
	if (!AbilitySystemComponent) return;

	if (GetLocalRole() == ROLE_Authority)
	{
		for (TSubclassOf<UGameplayAbility>& TestAbility : TestAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(TestAbility, GetCharacterLevel(), INDEX_NONE, this));
		}
	}
}

void ARPGCharacter::SetMeleeAbility()
{
	if (!AbilitySystemComponent) return;

	MeleeAbilitySpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(MeleeAbility, GetCharacterLevel(), INDEX_NONE, this));
}

// Called every frame
void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARPGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!AbilitySystemComponent) return;

	if (EnableTestAbilities) SetTestAbilities();
	
	ApplyDefaultAttributeEffects();
	SetMeleeAbility();
}

// Implement the IAbilitySystemInterface
UAbilitySystemComponent* ARPGCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}



void ARPGCharacter::HandleHealthChange(float DeltaValue, AActor* Causer)
{
	OnHealthChanged(DeltaValue, Causer);
	if (GetHealth() <= 0.0f)
	{
		OnDead();
	}
}

void ARPGCharacter::HandleStaminaChange(float DeltaValue, AActor* Causer)
{
	OnStaminaChanged(DeltaValue, Causer);
}

void ARPGCharacter::HandleAdrenalineChange(float DeltaValue, AActor* Causer)
{
	OnAdrenalineChanged(DeltaValue, Causer);
}

void ARPGCharacter::HandleExperiencePointsChange(float DeltaValue)
{
	OnExperiencePointsChanged(DeltaValue);
}

void ARPGCharacter::HandleCharacterLevelUp()
{
	CharacterLevel += 1;
	RemoveDefaultAttributeEffects();
	ApplyDefaultAttributeEffects();
	OnCharacterLevelUp();
}

void ARPGCharacter::ApplyDefaultAttributeEffects()
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	for (TSubclassOf<UGameplayEffect>& DefaultEffect : DefaultAttributeEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultEffect, CharacterLevel, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create default attribute effect spec for %s"), *GetName());
		}
	}
	
}

void ARPGCharacter::RemoveDefaultAttributeEffects()
{
	FGameplayEffectQuery Query;
	Query.EffectSource = this;
	AbilitySystemComponent->RemoveActiveEffects(Query);
}

