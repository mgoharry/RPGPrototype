// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TestRPG/Interfaces/Killable.h"
#include "TestRPG/Weapons/Lightsaber.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping Component"));

	PlayerStatsComponent = CreateDefaultSubobject<UPlayerStatsComponent>(TEXT("Player Stats Component"));

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));

	CustomCharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Custom Character Mesh"));
	CustomCharacterMesh->SetupAttachment(GetMesh());

	LightSaberActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("LightSaber Child Actor Component"));
	LightSaberActor->SetupAttachment(CustomCharacterMesh, FName("hand_r"));

	SwordTraceStartPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Sword Trace Start Point"));
	SwordTraceStartPoint->SetupAttachment(LightSaberActor);

	SwordTraceEndPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Sword Trace End Point"));
	SwordTraceEndPoint->SetupAttachment(LightSaberActor);

	HitTracePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Hit Trace Point"));
	HitTracePoint->SetupAttachment(RootComponent);

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.f, 0.0f);


}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	if (Controller && PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetLocalViewingPlayerController(), PlayerHUDWidgetClass);
		PlayerHUDWidget->AddToViewport();
	}

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	if (ALightsaber* LightSaber = Cast<ALightsaber>(LightSaberActor->GetChildActor()))
	{
		LightSaberREF = LightSaber;
	}
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MoveVector.Y);
		AddMovementInput(RightDirection, MoveVector.X);
	}
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(-LookAxis.X * LookSpeedX);
		AddControllerPitchInput(LookAxis.Y * LookSpeedY);
	}
}

void AMainCharacter::StartJump()
{
	if (bIsCrouching)
	{
		EndCrouch();
	}
	Jump();

	if (LightSaberREF) LightSaberREF->TurnOff();

	if (PlayerStatsComponent)
	{
		PlayerStatsComponent->DecreaseStamina(10);
	}
}

void AMainCharacter::EndJump()
{
	StopJumping();
}

void AMainCharacter::StartSprint()
{
	if (bIsCrouching) return;

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	if (LightSaberREF) LightSaberREF->TurnOff();

	GetWorld()->GetTimerManager().ClearTimer(StaminaRechargeHandle);

	if (!StaminaDepleteHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(StaminaDepleteHandle, this, &ThisClass::StaminaDeplete, 0.1, true);
	}
}

void AMainCharacter::EndSprint()
{
	GetWorldTimerManager().SetTimer(EndSprintHandle, this, &AMainCharacter::BeginEndSprint, 0.01f, true);


	GetWorld()->GetTimerManager().ClearTimer(StaminaDepleteHandle);

	if (!StaminaRechargeHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(StaminaRechargeHandle, this, &ThisClass::StaminaRecharge, 0.1, true);
	}
}

void AMainCharacter::BeginEndSprint()
{
	if (GetCharacterMovement()->MaxWalkSpeed > WalkSpeed)
	{
		GetCharacterMovement()->MaxWalkSpeed -= 10;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(EndSprintHandle);
	}
}

void AMainCharacter::StartCrouch()
{
	bIsCrouching = true;
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeedCrouched;
	EndSprint();
}

void AMainCharacter::EndCrouch()
{
	bIsCrouching = false;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

}

void AMainCharacter::Vault()
{
	TArray<AActor*> ActorsToIgnore;
	for (int i = 0; i < 3; i++)
	{
		FHitResult Hit;
		FVector Start = GetActorLocation() + FVector(0, 0, i * 30);
		FVector End = Start + GetActorForwardVector() * 180;

		//Sphere trace for Horizontal scan of objects to vault
		if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 5, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true))
		{
			for (int j = 0; j < 5; j++)
			{
				FHitResult VerticalHit;
				FVector VerticalStart = Hit.Location + FVector(0, 0, 100) + GetActorForwardVector() * j * 50;
				FVector VerticalEnd = VerticalStart + FVector(0, 0, -100);

				//Sphere trace for vertical scan of objects to vault and the vault start location
				if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), VerticalStart, VerticalEnd, 10, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, VerticalHit, true))
				{
					if (j == 0)
					{
						VaultStartPosition = VerticalHit.Location;
						//DrawDebugSphere(GetWorld(), VaultStartPosition, 10, 10, FColor::Purple, false, 5, 0, 2);
					}

					VaultMiddlePosition = VerticalHit.Location;
					bCanWarp = true;
					//DrawDebugSphere(GetWorld(), VaultMiddlePosition, 10, 10, FColor::Yellow, false, 5, 0, 2);

				}
				else
				{
					//Line trace to get the exact point to motion warp and land on
					FHitResult LandHit;
					FVector TraceStart = VerticalHit.TraceStart + GetActorForwardVector() * 80;
					FVector TraceEnd = TraceStart - FVector(0, 0, 1000);
					if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStart, TraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, LandHit, true))
					{
						VaultLandPosition = LandHit.Location;
						break;
					}
				}
			}
			VaultMotionWarp();
			break;
		}
	}


}

void AMainCharacter::EndVault()
{
	bCanWarp = false;

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	SetActorEnableCollision(true);

	//Prevent character from landing far away
	VaultLandPosition = FVector(0, 0, 20000);
}

void AMainCharacter::VaultMotionWarp()
{
	if (bCanWarp && UKismetMathLibrary::InRange_FloatFloat(VaultLandPosition.Z, CustomCharacterMesh->GetComponentLocation().Z - 50, CustomCharacterMesh->GetComponentLocation().Z + 50))
	{

		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		SetActorEnableCollision(false);

		//Motion Warping for vault start location 
		FMotionWarpingTarget VaultStartWarp;
		VaultStartWarp.Name = FName("VaultStart");
		VaultStartWarp.Location = VaultStartPosition;
		VaultStartWarp.Rotation = GetActorRotation();
		MotionWarpingComponent->AddOrUpdateWarpTarget(VaultStartWarp);

		//Motion Warping for vault middle location 
		FMotionWarpingTarget VaultMiddleWarp;
		VaultMiddleWarp.Name = FName("VaultMiddle");
		VaultMiddleWarp.Location = VaultMiddlePosition;
		VaultMiddleWarp.Rotation = GetActorRotation();
		MotionWarpingComponent->AddOrUpdateWarpTarget(VaultMiddleWarp);

		//Motion Warping for vault land location 
		FMotionWarpingTarget VaultLandWarp;
		VaultLandWarp.Name = FName("VaultLand");
		VaultLandWarp.Location = VaultLandPosition;
		VaultLandWarp.Rotation = GetActorRotation();
		MotionWarpingComponent->AddOrUpdateWarpTarget(VaultLandWarp);

		//PlayAnimMontage(VaultMontage);

		FTimerHandle VaultHandle;
		GetWorld()->GetTimerManager().SetTimer(VaultHandle, this, &ThisClass::EndVault, PlayAnimMontage(VaultMontage) - 0.7);
	}
}

void AMainCharacter::StealthKill()
{
	TArray<AActor*> OverlappingActors;

	FVector OutREFLocation;
	FRotator OutRotation;

	GetOverlappingActors(OverlappingActors);

	for (auto OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->GetClass()->ImplementsInterface(UKillable::StaticClass()))
		{
			Cast<IKillable>(OverlappingActor)->StealthKilled(OutREFLocation, OutRotation);

			FMotionWarpingTarget StealthWarp;
			StealthWarp.Name = FName("StealthWarp");
			StealthWarp.Location = OutREFLocation;
			StealthWarp.Rotation = OutRotation;
			MotionWarpingComponent->AddOrUpdateWarpTarget(StealthWarp);

			PlayAnimMontage(StealthKillMontage);

			if (LightSaberREF) LightSaberREF->TurnOn();

		}
	}
}

void AMainCharacter::ForceChoke()
{

	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> Hits;

	FVector Start = GetPawnViewLocation();
	FVector End = Start + GetControlRotation().Vector() * 600;

	if (UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Start, End, 50, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hits, true))
	{
		for (auto Hit : Hits)
		{
			if (Hit.GetActor()->GetClass()->ImplementsInterface(UKillable::StaticClass()))
			{
				Cast<IKillable>(Hit.GetActor())->ForceChoked(GetActorLocation() + GetActorForwardVector() * 600, GetActorLocation());

				FMotionWarpingTarget ChokeWarp;
				ChokeWarp.Name = FName("ChokeWarp");
				ChokeWarp.Location = GetActorLocation();
				ChokeWarp.Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Hit.GetActor()->GetActorLocation());
				MotionWarpingComponent->AddOrUpdateWarpTarget(ChokeWarp);

				PlayAnimMontage(ForceChokeMontage);
			}

		}
	}

}

void AMainCharacter::SwordAttack()
{

	if (CombatComponent)
	{
		CombatComponent->SwordAttackStart();
		if (LightSaberREF) LightSaberREF->TurnOn();

	}
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (PlayerStatsComponent) {
		if (PlayerStatsComponent->DecreaseHealth(DamageAmount))
		{
			CustomCharacterMesh->SetAllBodiesSimulatePhysics(true);
			DisableInput(GetLocalViewingPlayerController());
		}
		else
		{
			//Hit Effects
		}
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

}

void AMainCharacter::StaminaDeplete()
{
	if (PlayerStatsComponent && GetCharacterMovement()->Velocity.Size() > 0)
		if (PlayerStatsComponent->DecreaseStamina(1)) {
			GetWorld()->GetTimerManager().ClearTimer(StaminaDepleteHandle);

			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
}

void AMainCharacter::StaminaRecharge()
{
	if (PlayerStatsComponent)
		if (PlayerStatsComponent->IncreaseStamina(1))
			GetWorld()->GetTimerManager().ClearTimer(StaminaRechargeHandle);
}

void AMainCharacter::LightSaberOnOff()
{
	if (LightSaberREF) {
		LightSaberREF->TurnOnOff();
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMainCharacter::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMainCharacter::EndJump);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ThisClass::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::EndSprint);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ThisClass::EndCrouch);

		EnhancedInputComponent->BindAction(VaultAction, ETriggerEvent::Started, this, &ThisClass::Vault);

		EnhancedInputComponent->BindAction(StealthAction, ETriggerEvent::Started, this, &ThisClass::StealthKill);

		EnhancedInputComponent->BindAction(ForceChokeAction, ETriggerEvent::Started, this, &ThisClass::ForceChoke);

		EnhancedInputComponent->BindAction(SwordAttackAction, ETriggerEvent::Started, this, &ThisClass::SwordAttack);

		EnhancedInputComponent->BindAction(LSAction, ETriggerEvent::Started, this, &ThisClass::LightSaberOnOff);



	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("failed to find an Enhanced Input component!"));
	}
}

