// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatsComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TESTRPG_API UPlayerStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina, meta = (AllowPrivateAccess = "true"))
	float CurrentStamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina, meta = (AllowPrivateAccess = "true"))
	float MaxStamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Leveling, meta = (AllowPrivateAccess = "true"))
	float XP = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Leveling, meta = (AllowPrivateAccess = "true"))
	float MaxXP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Leveling, meta = (AllowPrivateAccess = "true"))
	int CurrentLevel = 1;

	class AMainCharacter* MainCharacter;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void IncreaseHealth(float Value);

	void IncreaseMaxHealth(float Value);

	bool DecreaseHealth(float Value);



	bool IncreaseStamina(float Value);

	void IncreaseMaxStamina(float Value);

	bool DecreaseStamina(float Value);

};
