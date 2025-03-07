// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TestRPG/Characters/MainCharacter.h"
#include "MainCharacterAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class TESTRPG_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMainCharacter* MainCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float YawDelta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCrouching;

	FRotator LastFrameRotation;
};
