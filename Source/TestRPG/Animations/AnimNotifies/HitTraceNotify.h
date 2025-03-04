// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "HitTraceNotify.generated.h"

/**
 *
 */
UCLASS()
class TESTRPG_API UHitTraceNotify : public UAnimNotify
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
