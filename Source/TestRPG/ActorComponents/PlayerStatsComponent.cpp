// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatsComponent.h"
#include "TestRPG/Characters/MainCharacter.h"

// Sets default values for this component's properties
UPlayerStatsComponent::UPlayerStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MainCharacter = Cast<AMainCharacter>(GetOwner());

	// ...
}


// Called when the game starts
void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	MainCharacter = Cast<AMainCharacter>(GetOwner());

}


// Called every frame
void UPlayerStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerStatsComponent::IncreaseHealth(float Value)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Value, 0.f, MaxHealth);

	if (MainCharacter)
		MainCharacter->PlayerHUDWidget->HealthBar->SetPercent(CurrentHealth / MaxHealth);
}

void UPlayerStatsComponent::IncreaseMaxHealth(float Value)
{
	MaxHealth += Value;
	CurrentHealth = MaxStamina;

	if (MainCharacter)
		MainCharacter->PlayerHUDWidget->HealthBar->SetPercent(CurrentHealth / MaxHealth);
}

bool UPlayerStatsComponent::DecreaseHealth(float Value)
{
	CurrentHealth = FMath::Clamp(CurrentStamina - Value, 0.f, MaxStamina);

	if (MainCharacter)
		MainCharacter->PlayerHUDWidget->HealthBar->SetPercent(CurrentHealth / MaxHealth);

	return CurrentHealth <= 0;
}

bool UPlayerStatsComponent::IncreaseStamina(float Value)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + Value, 0.f, MaxStamina);

	if (MainCharacter)
		MainCharacter->PlayerHUDWidget->StaminaBar->SetPercent(CurrentStamina / MaxStamina);

	return CurrentStamina >= 100;
}

void UPlayerStatsComponent::IncreaseMaxStamina(float Value)
{
	MaxStamina += Value;
	CurrentStamina = MaxStamina;

	if (MainCharacter)
		MainCharacter->PlayerHUDWidget->StaminaBar->SetPercent(CurrentStamina / MaxStamina);
}

bool UPlayerStatsComponent::DecreaseStamina(float Value)
{
	CurrentStamina = FMath::Clamp(CurrentStamina - Value, 0.f, MaxStamina);


	if (MainCharacter)
		MainCharacter->PlayerHUDWidget->StaminaBar->SetPercent(CurrentStamina / MaxStamina);

	return CurrentStamina <= 0;
}

