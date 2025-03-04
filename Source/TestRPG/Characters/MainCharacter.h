// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "TestRPG/TestRPGCharacter.h"
#include "TestRPG/ActorComponents/CombatComponent.h"
#include "TestRPG/ActorComponents/PlayerStatsComponent.h"
#include "TestRPG/UI/PlayerHUDWidget.h"
#include "MainCharacter.generated.h"

class ALightsaber;

UCLASS()
class TESTRPG_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPlayerStatsComponent* PlayerStatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* LightSaberActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* SwordTraceStartPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* SwordTraceEndPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* HitTracePoint;
	//Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowprivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VaultAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* StealthAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ForceChokeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwordAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LSAction;

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void StartJump();

	void EndJump();

	void StartSprint();

	void EndSprint();

	void BeginEndSprint();

	void StartCrouch();

	void EndCrouch();

	void Vault();

	void EndVault();

	void VaultMotionWarp();

	void StealthKill();

	void ForceChoke();

	void SwordAttack();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	bool bCanWarp;

	void StaminaDeplete();

	void StaminaRecharge();

	FTimerHandle StaminaDepleteHandle;

	FTimerHandle StaminaRechargeHandle;

	void LightSaberOnOff();

	UPROPERTY()
	ALightsaber* LightSaberREF;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controls)
	float LookSpeedX = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controls)
	float LookSpeedY = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	float WalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mechanics)
	float SprintSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationMontages)
	UAnimMontage* VaultMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationMontages)
	UAnimMontage* StealthKillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationMontages)
	UAnimMontage* ForceChokeMontage;

	bool bIsCrouching;

	FTimerHandle EndSprintHandle;

	FVector VaultStartPosition;
	FVector VaultMiddlePosition;
	FVector VaultLandPosition = FVector(0, 0, 20000);

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UCombatComponent* GetCombatComponent() const { return CombatComponent; }

	FORCEINLINE FVector GetSwordTraceStart() const { return SwordTraceStartPoint->GetComponentLocation(); }
	FORCEINLINE FVector GetSwordTraceEnd() const { return SwordTraceEndPoint->GetComponentLocation(); }
	FORCEINLINE FVector GetHitTracePoint() const { return HitTracePoint->GetComponentLocation(); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;
	UPlayerHUDWidget* PlayerHUDWidget;



};
