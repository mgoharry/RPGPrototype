// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (MainCharacter == nullptr) return;

	Velocity = MainCharacter->GetVelocity();

	Speed = Velocity.Size();

	bIsFalling = MainCharacter->GetCharacterMovement()->IsFalling();

	bIsCrouching = MainCharacter->bIsCrouching;

	FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(MainCharacter->GetActorRotation(), LastFrameRotation);

	YawDelta = FMath::FInterpTo(YawDelta, DeltaRotator.Yaw / DeltaSeconds / 10, DeltaSeconds, 10);

	LastFrameRotation = MainCharacter->GetActorRotation();
}
