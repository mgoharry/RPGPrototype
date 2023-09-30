// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class AMainCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TESTRPG_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	AMainCharacter* MainCharacter;

	bool bIsAttacking;

	bool bIsSavingAttack;

	bool bCanAttack;

	int AttackIndex;

	void SwitchOnAttackIndex();

	void PlayAttackAnimation(int InAttackIndex);


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void CharacterPlayAnimMontage(UAnimMontage* InAnimMontage);

	void SwordAttackStart();

	void AttackCombo();

	void SwordAttackEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UAnimMontage* AttackAnim1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UAnimMontage* AttackAnim2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UAnimMontage* AttackAnim3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UAnimMontage* KickAnim;

};
