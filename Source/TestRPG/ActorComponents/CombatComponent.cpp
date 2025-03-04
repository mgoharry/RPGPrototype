// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "Engine/DamageEvents.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TestRPG/Characters/EnemyCharacter.h"
#include "TestRPG/Characters/MainCharacter.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	MainCharacter = Cast<AMainCharacter>(GetOwner());

	bCanAttack = true;

}


void UCombatComponent::SwitchOnAttackIndex()
{
	if (!MainCharacter) {
		return;
	}

	switch (AttackIndex)
	{
	case 0:
		PlayAttackAnimation(1);
		break;
	case 1:
		PlayAttackAnimation(2);
		break;
	case 2:
		PlayAttackAnimation(3);
		break;
	case 3:
		PlayAttackAnimation(0);
		break;
	default:
		PlayAttackAnimation(3);

	}
}



// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::SwordAttackStart()
{

	if (bCanAttack)
	{
		if (bIsAttacking)
		{
			bIsSavingAttack = true;
		}
		else
		{
			bIsAttacking = true;
			SwitchOnAttackIndex();
		}
	}
}

void UCombatComponent::AttackCombo()
{
	if (bIsSavingAttack)
	{
		bIsSavingAttack = false;

		SwitchOnAttackIndex();
	}
	else
	{
		SwordAttackEnd();
	}
}

void UCombatComponent::SwordAttackEnd()
{
	bIsAttacking = false;
	AttackIndex = 0;
}

void UCombatComponent::SwordTrace_Start()
{
	GetWorld()->GetTimerManager().SetTimer(SwordTraceHandle, this, &UCombatComponent::SwordTrace_Loop, 0.001f, true);
}

void UCombatComponent::SwordTrace_Loop()
{
	if (!MainCharacter) return;

	FDamageEvent SwordDamage;
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;

	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), MainCharacter->GetSwordTraceStart(), MainCharacter->GetSwordTraceEnd(), 12, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true, FColor::Red, FColor::Green, 0.2))
	{
		if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Hit.GetActor()))
		{
			Enemy->TakeDamage(10, SwordDamage, MainCharacter->GetController(), GetOwner());
		}
	}
}

void UCombatComponent::SwordTrace_End()
{
	GetWorld()->GetTimerManager().ClearTimer(SwordTraceHandle);
}

void UCombatComponent::ForwardHitTrace()
{
	if (!MainCharacter) return;

	FDamageEvent SwordDamage;
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;

	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), MainCharacter->GetHitTracePoint(), MainCharacter->GetHitTracePoint(), 40, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true, FColor::Red, FColor::Green, 0.2))
	{
		if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Hit.GetActor()))
		{
			Enemy->TakeDamage(10, SwordDamage, MainCharacter->GetController(), GetOwner());
		}
	}
}

void UCombatComponent::PlayAttackAnimation(int InAttackIndex)
{
	FTimerHandle AttackTimerHandle;
	FTimerDelegate AttackDelegate;

	switch (InAttackIndex)
	{

	case 0:
		AttackDelegate.BindUFunction(this, FName("CharacterPlayAnimMontage"), AttackAnim3);
		break;
	case 1:
		AttackDelegate.BindUFunction(this, FName("CharacterPlayAnimMontage"), AttackAnim1);

		break;
	case 2:
		AttackDelegate.BindUFunction(this, FName("CharacterPlayAnimMontage"), AttackAnim2);
		break;
	case 3:
		AttackDelegate.BindUFunction(this, FName("CharacterPlayAnimMontage"), KickAnim);

		break;

	default:
		AttackDelegate.BindUFunction(this, FName("CharacterPlayAnimMontage"), AttackAnim1);

	}

	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, AttackDelegate, 0.1, false);

	AttackIndex = InAttackIndex;
}


void UCombatComponent::CharacterPlayAnimMontage(UAnimMontage* InAnimMontage)
{
	MainCharacter->PlayAnimMontage(InAnimMontage);

}


