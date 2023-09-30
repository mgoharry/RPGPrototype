// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Lightsaber.generated.h"

UCLASS()
class TESTRPG_API ALightsaber : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightsaber();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HandleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* EdgeMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector TurnedOnScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector TurnedOffScale;


	FTimeline LightSaberTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* LightSaberCurveFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase* LightSaberSoundOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase* LightSaberSoundOff;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void LightSaberTimelineProgress(float Value);

	bool bIsOn = false;

	void TurnOnOff();

	void TurnOn();

	void TurnOff();

};
