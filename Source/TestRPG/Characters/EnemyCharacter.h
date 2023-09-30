// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "TestRPG/Interfaces/Stealthable.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class TESTRPG_API AEnemyCharacter : public ACharacter, public IStealthable
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMotionWarpingComponent* MotionWarpingComp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* StealthKillSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* StealthKillREF;


	void BeginRagdoll();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void StealthKilled(FVector& outREFLocation, FRotator& outRotation) override;

	virtual void ForceChoked(FVector Location, FVector PlayerLocation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationMontages)
	UAnimMontage* StealthKilledMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationMontages)
	UAnimMontage* ForceChokedMontage;

};
