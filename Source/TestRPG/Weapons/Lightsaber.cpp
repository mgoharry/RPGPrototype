
// Fill out your copyright notice in the Description page of Project Settings.


#include "Lightsaber.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ALightsaber::ALightsaber()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	RootComponent = CapsuleComponent;

	HandleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle Mesh"));
	HandleMesh->SetupAttachment(RootComponent);

	EdgeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Edge Mesh"));
	EdgeMesh->SetupAttachment(HandleMesh);
}

// Called when the game starts or when spawned
void ALightsaber::BeginPlay()
{
	Super::BeginPlay();

}

void ALightsaber::LightSaberTimelineProgress(float Value)
{
	EdgeMesh->SetRelativeScale3D(FMath::Lerp(TurnedOffScale, TurnedOnScale, Value));
}

// Called every frame
void ALightsaber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LightSaberTimeline.IsPlaying())
	{
		LightSaberTimeline.TickTimeline(DeltaTime);
	}

}

void ALightsaber::TurnOnOff()
{
	if (bIsOn) {
		TurnOff();
	}
	else
	{
		if (LightSaberCurveFloat)
		{
			FOnTimelineFloat TimeLineProgress;
			TimeLineProgress.BindUFunction(this, TEXT("LightSaberTimelineProgress"));

			LightSaberTimeline.AddInterpFloat(LightSaberCurveFloat, TimeLineProgress);

			LightSaberTimeline.SetLooping(false);

			LightSaberTimeline.PlayFromStart();

			LightSaberTimeline.SetPlayRate(1);

			bIsOn = true;

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), LightSaberSoundOn, GetActorLocation(), GetActorRotation());
		}
	}
}

void ALightsaber::TurnOn()
{

	if (LightSaberCurveFloat && !bIsOn)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, TEXT("LightSaberTimelineProgress"));

		LightSaberTimeline.AddInterpFloat(LightSaberCurveFloat, TimeLineProgress);

		LightSaberTimeline.SetLooping(false);

		LightSaberTimeline.Play();

		LightSaberTimeline.SetPlayRate(1);

		bIsOn = true;

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), LightSaberSoundOn, GetActorLocation(), GetActorRotation());
	}

}

void ALightsaber::TurnOff()
{
	if (bIsOn && LightSaberCurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, TEXT("LightSaberTimelineProgress"));

		LightSaberTimeline.AddInterpFloat(LightSaberCurveFloat, TimeLineProgress);

		LightSaberTimeline.SetLooping(false);

		LightSaberTimeline.Reverse();

		LightSaberTimeline.SetPlayRate(1);

		bIsOn = false;

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), LightSaberSoundOff, GetActorLocation(), GetActorRotation());

	}
}

