// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StealthKillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Stealth Sphere"));
	StealthKillSphere->SetupAttachment(RootComponent);

	StealthKillREF = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Stealth REF"));
	StealthKillREF->SetupAttachment(RootComponent);

	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping Comp"));

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemyCharacter::BeginRagdoll()
{
	GetMesh()->SetAllBodiesSimulatePhysics(true);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::StealthKilled(FVector& outREFLocation, FRotator& outRotation)
{
	IStealthable::StealthKilled(outREFLocation, outRotation);

	outREFLocation = StealthKillREF->GetComponentLocation();

	outRotation = GetActorRotation();

	PlayAnimMontage(StealthKilledMontage);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("STEALTH KILL"));

	FTimerHandle RagdollHandle;
	GetWorldTimerManager().SetTimer(RagdollHandle, this, &ThisClass::BeginRagdoll, 2.f);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StealthKillSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyCharacter::ForceChoked(FVector Location, FVector PlayerLocation)
{
	IStealthable::ForceChoked(Location, PlayerLocation);
	FMotionWarpingTarget ChokeWarp;
	ChokeWarp.Name = FName("ChokeWarp");
	ChokeWarp.Location = GetActorLocation();
	ChokeWarp.Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerLocation);
	MotionWarpingComp->AddOrUpdateWarpTarget(ChokeWarp);

	PlayAnimMontage(ForceChokedMontage);

	FTimerHandle RagdollHandle;
	GetWorldTimerManager().SetTimer(RagdollHandle, this, &ThisClass::BeginRagdoll, 3.f);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StealthKillSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

